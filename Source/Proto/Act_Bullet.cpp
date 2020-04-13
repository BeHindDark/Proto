// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_Bullet.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Act_WeaponBase.h"
#include "Act_ProjectileWeaponBase.h"
#include "kismet/GameplayStatics.h"
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
	ExplodeAudio = CreateDefaultSubobject<USoundCue>(TEXT("ExplodeAudio"));
	ExplodeFX = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplodeFX"));
	
	RootComponent = DefaultSceneRoot;

	SpanTime = 100000;

	//static ConstructorHelpers::FObjectFinder<USoundCue> EXPLODE_FX(TEXT(""));

	//총알 메쉬 설정
	BulletMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Bullet(TEXT("/Game/Mech_Constructor_Spiders/Meshes/Projectile_Bullet.Projectile_Bullet"));
	if(SM_Bullet.Succeeded())
	{
		BulletMesh->SetStaticMesh(SM_Bullet.Object);
	}
	BulletMesh->SetRelativeLocationAndRotation(FVector(-10.0f, 0.0f, 0.0f), FRotator(0.0f,-90.0f,0.0f));
	BulletMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//충돌 설정
	BulletCollision->SetupAttachment(RootComponent);
	BulletCollision->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));
	BulletCollision->SetCapsuleSize(7.0f, 30.0f);
	
	BulletCollision->SetCollisionProfileName(TEXT("Projectile"));
	//BulletCollision->OnComponentHit.AddDynamic(this, &AAct_Bullet::HitCheck);
	BulletCollision->SetGenerateOverlapEvents(true);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this,&AAct_Bullet::BeginOverlap);
	BulletCollision->bAutoActivate = false;
	BulletCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	//폭발이펙트 설정

	ExplodeFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EXPLODE_EFFECT(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (EXPLODE_EFFECT.Succeeded()) {
		ExplodeFX->SetTemplate(EXPLODE_EFFECT.Object);
		ExplodeFX->SetAutoActivate(false);
	}
	//파티클 설정
	TracerFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Tracer(TEXT("/Game/CustomFX/BulletTracer.BulletTracer"));
	if(PS_Tracer.Succeeded())
	{
		TracerFX->SetTemplate(PS_Tracer.Object);
	}
	TracerFX->SetRelativeLocation(FVector(-20.0f,0.0f,0.0f));
	TracerFX->bAutoActivate = false;

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
	
	//총알액터 수명설정
	SetLifeSpan(SpanTime);
}

// Called every frame
void AAct_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AAct_Bullet::InitializeBullet_Validate(AAct_ProjectileWeaponBase* BOwner, AController* InputPlayerController, float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	return true;
}

void AAct_Bullet::InitializeBullet_Implementation(AAct_ProjectileWeaponBase* BOwner, AController* InputPlayerController, float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor) {
	ProjectileMovement->SetVelocityInLocalSpace(FVector(InitialSpeed, 0.0f, 0.0f));
	Damage = WeaponDamage;
	DamageInstigatorPlayer = InputPlayerController;
	
	//본인을 발사한 액터 제외
	//BulletCollision->MoveIgnoreActors.Add(BOwner);
	BulletCollision->IgnoreActorWhenMoving(BOwner, true);
	

	//본인을 발사한 무기 외 다른 무기들 제외	
	AAct_ProjectileWeaponBase* Weapon = Cast<AAct_ProjectileWeaponBase>(BOwner);
	
	if(IsValid(BOwner))
	{
		IgnoresArray.AddUnique(BOwner);
		BOwner->WeaponSkeletalMesh->IgnoreActorWhenMoving(this, true);
		if(IsValid(BOwner->GetWeaponControlSystem()))
		{
			for(FWeaponData WeaponData :BOwner->GetWeaponControlSystem()->WeaponDataArray)
			{
				if(IsValid(WeaponData.Weapon))
				{
					IgnoresArray.AddUnique(WeaponData.Weapon);
					if(WeaponData.Weapon!= BOwner)
					{
						//BulletCollision->MoveIgnoreActors.Add(WeaponData.Weapon);
						BulletCollision->IgnoreActorWhenMoving(WeaponData.Weapon, true);
					}
				}
				else
				{
					UE_LOG(Proto,Warning,TEXT("%s / %s : otherWeapon is not Valid"),*LINE_INFO,*GetNameSafe(this));
				}
			}
			//캐릭터로부터 플레이어컨트롤러를 받아온다.
			APawn* WeaponOwner = Cast<APawn>(BOwner->GetWeaponControlSystem()->GetOwner());
			if(IsValid(WeaponOwner))
			{
				IgnoresArray.AddUnique(WeaponOwner);
				//캐릭터 제외
				//BulletCollision->MoveIgnoreActors.Add(WeaponOwner);
				BulletCollision->IgnoreActorWhenMoving(WeaponOwner, true);
				
			}
			else
			{
				UE_LOG(Proto,Warning,TEXT("%s / %s : WeaponOwner is not Valid"),*LINE_INFO,*GetNameSafe(this));
			}
		}
		else
		{
			UE_LOG(Proto,Warning,TEXT("%s / %s : Fail to Get WeaponControlSystem"),*LINE_INFO,*GetNameSafe(this));
		}
		
	}
	else
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : Fail to Get Owner"),*LINE_INFO,*GetNameSafe(this));
	}

	if(!TracerColor.Equals(NewTracerColor, 0.000001f))
	{
		TracerColor = NewTracerColor;
	}

	UMaterialInstanceDynamic* MaterialInstance = TracerFX->CreateDynamicMaterialInstance(0);
	MaterialInstance->SetVectorParameterValue(FName(TEXT("TracerBaseColor")), TracerColor);
	TracerFX->SetMaterial(0, MaterialInstance);
	TracerFX->Activate();
	ProjectileMovement->Activate();
	BulletCollision->Activate();
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
	//Destroy();
}

void AAct_Bullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	
	if (OtherActor != nullptr)
	{
		if(IgnoresArray.Find(OtherActor)!=INDEX_NONE)
		{
			return;
		}
		UE_LOG(Proto,Warning,TEXT("%s / %s : OverLAP with (%s)"),*LINE_INFO,*GetNameSafe(this), *GetNameSafe(OtherActor));
		GEngine->AddOnScreenDebugMessage(10, 5.0f, FColor::Red, FString::Printf(TEXT("overlap Component : %s"), *OtherActor->GetName()));
		ProjectileMovement->SetVelocityInLocalSpace(FVector::ZeroVector);
		ProjectileMovement->StopMovementImmediately();
		if (GetLocalRole() >= ROLE_Authority)
		{
			UGameplayStatics::ApplyDamage(OtherActor, Damage, DamageInstigatorPlayer, this, UDamageType::StaticClass());
		}
		//FVector DamageDirection = SweepResult.ImpactPoint - GetActorLocation();
		//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, DamageDirection, SweepResult, DamageInstigatorPlayer, GetOwner(), UDamageType::StaticClass());
		
		ExplodeFX->Activate(true);
		TracerFX->DeactivateSystem();
		BulletCollision->SetHiddenInGame(true, true);
		SetActorEnableCollision(false);
		BulletMesh->SetVisibility(false);
		ProjectileMovement->Deactivate();

		ExplodeFX->OnSystemFinished.AddDynamic(this, &AAct_Bullet::StopFX);
	}
	
}

