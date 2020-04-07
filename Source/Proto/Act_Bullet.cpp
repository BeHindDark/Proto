// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Act_WeaponBase.h"
#include "WeaponControlSystem.h"


// Sets default values
AAct_Bullet::AAct_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//���ø����̼� ����
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BulletCollision"));
	TracerFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TracerFX"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	RootComponent = DefaultSceneRoot;

	//�Ѿ� �޽� ����
	BulletMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Bullet(TEXT("/Game/Mech_Constructor_Spiders/Meshes/Projectile_Bullet.Projectile_Bullet"));
	if(SM_Bullet.Succeeded())
	{
		BulletMesh->SetStaticMesh(SM_Bullet.Object);
	}
	BulletMesh->SetRelativeLocationAndRotation(FVector(-10.0f, 0.0f, 0.0f), FRotator(0.0f,-90.0f,0.0f));

	//�浹 ����
	BulletCollision->SetupAttachment(RootComponent);
	BulletCollision->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	BulletCollision->SetCapsuleSize(7.0f, 30.0f);

	//�浹���� ���� �ؾ���

	//������ �߻��� ���� ����
	BulletCollision->MoveIgnoreActors.Add(GetOwner());

	//������ �߻��� ���� �� �ٸ� ����� ����
	AAct_WeaponBase* Weapon = Cast<AAct_WeaponBase>(GetOwner());
	if(IsValid(Weapon))
	{
		for(FWeaponData WeaponData :Weapon->GetWeaponControlSystem()->WeaponDataArray)
		{
			if(IsValid(WeaponData.Weapon))
			{
				if(WeaponData.Weapon!=GetOwner())
				{
					BulletCollision->MoveIgnoreActors.Add(WeaponData.Weapon);
				}
			}
		}

		//ĳ���ͷκ��� �÷��̾���Ʈ�ѷ��� �޾ƿ´�.
		APawn* WeaponOwner = Cast<APawn>(Weapon->GetWeaponControlSystem()->GetOwner());
		if(IsValid(WeaponOwner))
		{
			//ĳ���� ����
			BulletCollision->MoveIgnoreActors.Add(WeaponOwner);

			if(IsValid(WeaponOwner->GetController()))
			{
				DamageInstigatorPlayer = WeaponOwner->GetController();
			}
		}
	}
	
	
	

	BulletCollision->SetGenerateOverlapEvents(false);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &AAct_Bullet::BeginOverlap);

	//��ƼŬ ����
	TracerFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Tracer(TEXT("/Game/CustomFX/BulletTracer.BulletTracer"));
	if(PS_Tracer.Succeeded())
	{
		TracerFX->SetTemplate(PS_Tracer.Object);
	}
	TracerFX->SetRelativeLocation(FVector(-20.0f,0.0f,0.0f));

	//ProjectielMovement����
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 30000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bAutoActivate = false;
	
}

// Called when the game starts or when spawned
void AAct_Bullet::BeginPlay()
{
	Super::BeginPlay();

	ProjectileMovement->Velocity = ProjectileVelocity;
	
}

// Called every frame
void AAct_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAct_Bullet::InitializeBullet_Validate(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	return true;
}

void AAct_Bullet::InitializeBullet_Implementation(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	ProjectileMovement->SetVelocityInLocalSpace(FVector(InitialSpeed, 0.0f, 0.0f));
	Damage = WeaponDamage;
	ProjectileMovement->bAutoActivate = true;
}

void AAct_Bullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	ProjectileMovement->StopMovementImmediately();

	//��Ƽĳ��Ʈ : �޽� �� ����ź ����Ʈ ����, �ǰ� ����Ʈ �� ���� ���

	//Instigator�� ���ϴ� �� �κ��� ���� Weapon������ ���� ó���ϴ� �� ���ƺ���.
	//WeaponBase�� GetInstigator�Լ��� ���� AController�� ���ϰ� �ϰ�
	//���� �� ������ �Ѱ��ִ°� ���ƺ���.
	//�ֳĸ� �Ѿ��� �������� �� �̹� ������ �׾��� ����... ���� �����ϱ�?
	/*
	AAct_WeaponBase* Weapon = Cast<AAct_WeaponBase>(GetOwner());
	if(!IsValid(Weapon))
	{
		CHECK_LOG(!IsValid(Weapon));
		return;
	}

	APawn* WeaponOwenr = Cast<APawn>(Weapon->GetWeaponControlSystem()->GetOwner());
	if(!IsValid(WeaponOwenr))
	{
		CHECK_LOG(!IsValid(WeaponOwner));
		return;
	}

	AController* BulletInstigator = Cast<AController>(WeaponOwenr->GetController());
	if(!IsValid(BulletInstigator))
	{
		CHECK_LOG(!IsValid(WeaponOwner));
		return;
	}
	UGameplayStatics::ApplyPointDamage(OtherActor, Damage, GetVelocity(), SweepResult, BulletInstigator, GetOwner(),UDamageType::StaticClass());
	//UGameplayStatics::ApplyDamage(OtherActor, Damage, Instigator, GetOwner(), UDamageType::StaticClass());
	*/
}

