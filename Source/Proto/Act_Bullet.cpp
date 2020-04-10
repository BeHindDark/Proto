// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Act_WeaponBase.h"
#include "kismet/GameplayStatics.h"
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
	ExplodeAudio = CreateDefaultSubobject<USoundCue>(TEXT("ExplodeAudio"));
	ExplodeFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplodeFX"));
	
	RootComponent = DefaultSceneRoot;

	SpanTime = 10;

	//static ConstructorHelpers::FObjectFinder<USoundCue> EXPLODE_FX(TEXT(""));

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

	//�浹 �ݸ��� ����
	BulletCollision->SetCollisionProfileName(TEXT("Projectile"));
	//BulletCollision->OnComponentHit.AddDynamic(this, &AAct_Bullet::HitCheck);
	BulletCollision->SetGenerateOverlapEvents(true);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this,&AAct_Bullet::BeginOverlap);
	BulletCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//������ �߻��� ���� ����
	BulletCollision->MoveIgnoreActors.Add(GetOwner());

	//������ �߻��� ���� �� �ٸ� ����� ����
	AAct_WeaponBase* Weapon = Cast<AAct_WeaponBase>(GetOwner());
	if(IsValid(Weapon))
	{
		UE_LOG(Proto, Warning, TEXT("1"));
		for(FWeaponData WeaponData :Weapon->GetWeaponControlSystem()->WeaponDataArray)
		{
			if(IsValid(WeaponData.Weapon))
			{
				UE_LOG(Proto,Warning,TEXT("2"));
				if(WeaponData.Weapon!=GetOwner())
				{
					UE_LOG(Proto,Warning,TEXT("3"));
					BulletCollision->MoveIgnoreActors.Add(WeaponData.Weapon);
				}
			}
		}
		//ĳ���ͷκ��� �÷��̾���Ʈ�ѷ��� �޾ƿ´�.
		APawn* WeaponOwner = Cast<APawn>(Weapon->GetWeaponControlSystem()->GetOwner());
		if(IsValid(WeaponOwner))
		{
			//ĳ���� ����
			UE_LOG(Proto,Warning,TEXT("4"));
			BulletCollision->MoveIgnoreActors.Add(WeaponOwner);
		}
	}	

	

	//��������Ʈ ����

	ExplodeFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EXPLODE_EFFECT(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (EXPLODE_EFFECT.Succeeded()) {
		ExplodeFX->SetTemplate(EXPLODE_EFFECT.Object);
		ExplodeFX->SetAutoActivate(false);
	}
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

	//�Ѿ˾��� ������
	SetLifeSpan(SpanTime);
}

// Called every frame
void AAct_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAct_Bullet::InitializeBullet_Validate(AController* InputPlayerController, float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	return true;
}

void AAct_Bullet::InitializeBullet_Implementation(AController* InputPlayerController, float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	ProjectileMovement->SetVelocityInLocalSpace(FVector(InitialSpeed, 0.0f, 0.0f));
	Damage = WeaponDamage;
	ProjectileMovement->Activate();
	DamageInstigatorPlayer = InputPlayerController;
	BulletCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	if(!TracerColor.Equals(NewTracerColor, 0.000001f))
	{
		TracerColor = NewTracerColor;
	}
	TracerFX->GetMaterial()
}

void AAct_Bullet::HitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {

	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, FString::Printf(TEXT("Hit Component : %s"), *OtherActor->GetName()));

		ProjectileMovement->StopMovementImmediately();
		UGameplayStatics::ApplyDamage(OtherActor, Damage, DamageInstigatorPlayer, this, UDamageType::StaticClass());
	}
	

	ExplodeFX->Activate(true);
	BulletCollision->SetHiddenInGame(true, true);
	SetActorEnableCollision(false);

	ExplodeFX->OnSystemFinished.AddDynamic(this, &AAct_Bullet::StopFX);
}

bool AAct_Bullet::StopFX_Validate(UParticleSystemComponent* PSystem) {
	return true;
}

void AAct_Bullet::StopFX_Implementation(UParticleSystemComponent* PSystem) {
	Destroy();
}

void AAct_Bullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor != nullptr && OtherActor != this && OtherComponent != nullptr) {
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, FString::Printf(TEXT("Hit Component : %s"), *OtherActor->GetName()));

		ProjectileMovement->StopMovementImmediately();
		//UGameplayStatics::ApplyDamage(OtherActor, Damage, DamageInstigatorPlayer, this, UDamageType::StaticClass());
		FVector DamageDirection = SweepResult.ImpactPoint - GetActorLocation();
		UGameplayStatics::ApplyPointDamage(OtherActor, Damage, DamageDirection, SweepResult, DamageInstigatorPlayer, GetOwner(), UDamageType::StaticClass());
		
		ExplodeFX->Activate(true);
		BulletCollision->SetHiddenInGame(true, true);
		SetActorEnableCollision(false);

		ExplodeFX->OnSystemFinished.AddDynamic(this, &AAct_Bullet::StopFX);
	}
	
}

