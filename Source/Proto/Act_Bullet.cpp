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

	//리플리케이션 설정
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BulletCollision"));
	TracerFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TracerFX"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	
	RootComponent = DefaultSceneRoot;

	//총알 메쉬 설정
	BulletMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Bullet(TEXT("/Game/Mech_Constructor_Spiders/Meshes/Projectile_Bullet.Projectile_Bullet"));
	if(SM_Bullet.Succeeded())
	{
		BulletMesh->SetStaticMesh(SM_Bullet.Object);
	}
	BulletMesh->SetRelativeLocationAndRotation(FVector(-10.0f, 0.0f, 0.0f), FRotator(0.0f,-90.0f,0.0f));

	//충돌 설정
	BulletCollision->SetupAttachment(RootComponent);
	BulletCollision->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	BulletCollision->SetCapsuleSize(7.0f, 30.0f);

	//충돌설정 마저 해야함

	//본인을 발사한 액터 제외
	BulletCollision->MoveIgnoreActors.Add(GetOwner());

	//본인을 발사한 무기 외 다른 무기들 제외
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

		//캐릭터로부터 플레이어컨트롤러를 받아온다.
		APawn* WeaponOwner = Cast<APawn>(Weapon->GetWeaponControlSystem()->GetOwner());
		if(IsValid(WeaponOwner))
		{
			//캐릭터 제외
			BulletCollision->MoveIgnoreActors.Add(WeaponOwner);

			if(IsValid(WeaponOwner->GetController()))
			{
				DamageInstigatorPlayer = WeaponOwner->GetController();
			}
		}
	}
	
	
	

	BulletCollision->SetGenerateOverlapEvents(false);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &AAct_Bullet::BeginOverlap);

	//파티클 설정
	TracerFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Tracer(TEXT("/Game/CustomFX/BulletTracer.BulletTracer"));
	if(PS_Tracer.Succeeded())
	{
		TracerFX->SetTemplate(PS_Tracer.Object);
	}
	TracerFX->SetRelativeLocation(FVector(-20.0f,0.0f,0.0f));

	//ProjectielMovement설정
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

	//멀티캐스트 : 메쉬 및 예광탄 이펙트 끄기, 피격 이펙트 및 사운드 재생

	//Instigator를 구하는 이 부분은 차라리 Weapon쪽으로 빼서 처리하는 게 나아보임.
	//WeaponBase에 GetInstigator함수를 만들어서 AController를 구하게 하고
	//스폰 시 변수로 넘겨주는게 나아보임.
	//왜냐면 총알이 명중했을 떄 이미 주인이 죽었을 가능... 성도 있으니까?
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

