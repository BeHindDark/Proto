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

	SpanTime = 1000000;

	//static ConstructorHelpers::FObjectFinder<USoundCue> EXPLODE_FX(TEXT(""));

	//총알 메쉬 설정
	BulletMesh->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SM_Bullet(TEXT("/Game/Mech_Constructor_Spiders/Meshes/Projectile_Bullet.Projectile_Bullet"));
	if(SM_Bullet.Succeeded())
	{
		BulletMesh->SetStaticMesh(SM_Bullet.Object);
	}
	BulletMesh->SetRelativeLocationAndRotation(FVector(-10.0f,0.0f,0.0f),FRotator(0.0f,-90.0f,0.0f));


	//충돌 설정
	BulletCollision->SetupAttachment(RootComponent);
	BulletCollision->SetRelativeRotation(FRotator(-90.0f,0.0f,0.0f));
	BulletCollision->SetCapsuleSize(7.0f,30.0f);
	BulletCollision->bAutoActivate = false;
	BulletCollision->SetIsReplicated(true);

	BulletCollision->SetGenerateOverlapEvents(true);
	//폭발이펙트 설정

	ExplodeFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> EXPLODE_EFFECT(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if(EXPLODE_EFFECT.Succeeded()) {
		ExplodeFX->SetTemplate(EXPLODE_EFFECT.Object);
		ExplodeFX->SetAutoActivate(false);
	}
	//ExplodeFX->SetIsReplicated(true);

	//파티클 설정
	TracerFX->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UParticleSystem> PS_Tracer(TEXT("/Game/CustomFX/BulletTracer.BulletTracer"));
	if(PS_Tracer.Succeeded())
	{
		TracerFX->SetTemplate(PS_Tracer.Object);
	}
	TracerFX->SetRelativeLocation(FVector(-20.0f,0.0f,0.0f));
	TracerFX->bAutoActivate = false;
	//TracerFX->SetIsReplicated(true);

	//ProjectielMovement설정
	ProjectileMovement->InitialSpeed = 0.0f;
	ProjectileMovement->MaxSpeed = 30000.0f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bAutoActivate = false;
	ProjectileMovement->SetIsReplicated(true);

}

void AAct_Bullet::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	BulletMesh->SetCollisionProfileName(TEXT("NoCollision"));
	BulletCollision->SetCollisionProfileName(TEXT("Projectile"));
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this,&AAct_Bullet::BeginOverlap);
	BulletCollision->OnComponentHit.AddDynamic(this,&AAct_Bullet::HitCheck);
	BulletCollision->IgnoreActorWhenMoving(GetOwner(),true);
	AAct_ProjectileWeaponBase* Weapon = Cast<AAct_ProjectileWeaponBase>(GetOwner());

	if(IsValid(Weapon))
	{
		Weapon->WeaponSkeletalMesh->IgnoreActorWhenMoving(this,true);
		if(IsValid(Weapon->GetWeaponControlSystem()))
		{
			for(FWeaponData WeaponData :Weapon->GetWeaponControlSystem()->WeaponDataArray)
			{
				if(IsValid(WeaponData.Weapon))
				{

					if(WeaponData.Weapon!= Weapon)
					{
						//BulletCollision->MoveIgnoreActors.Add(WeaponData.Weapon);
						BulletCollision->IgnoreActorWhenMoving(WeaponData.Weapon,true);
					}
				}
				else
				{
					UE_LOG(Proto,Warning,TEXT("%s / %s : otherWeapon is not Valid"),*LINE_INFO,*GetNameSafe(this));
				}
			}
			//캐릭터로부터 플레이어컨트롤러를 받아온다.
			APawn* WeaponOwner = Cast<APawn>(Weapon->GetWeaponControlSystem()->GetOwner());
			if(IsValid(WeaponOwner))
			{
				//캐릭터 제외
				BulletCollision->IgnoreActorWhenMoving(WeaponOwner,true);
				DamageInstigatorPlayer = WeaponOwner->GetController();
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

bool AAct_Bullet::InitializeBullet_Validate(AAct_ProjectileWeaponBase* BOwner,AController* InputPlayerController,float InitialSpeed,float WeaponDamage,FLinearColor NewTracerColor)
{
	return true;
}

void AAct_Bullet::InitializeBullet_Implementation(AAct_ProjectileWeaponBase* BOwner,AController* InputPlayerController,float InitialSpeed,float WeaponDamage,FLinearColor NewTracerColor)
{
	ProjectileMovement->SetVelocityInLocalSpace(FVector(InitialSpeed,0.0f,0.0f));
	Damage = WeaponDamage;
	DamageInstigatorPlayer = InputPlayerController;

	if(!TracerColor.Equals(NewTracerColor,0.000001f))
	{
		TracerColor = NewTracerColor;
	}

	UMaterialInstanceDynamic* MaterialInstance = TracerFX->CreateDynamicMaterialInstance(0);
	MaterialInstance->SetVectorParameterValue(FName(TEXT("TracerBaseColor")),TracerColor);
	TracerFX->SetMaterial(0,MaterialInstance);
	TracerFX->Activate();	
	BulletCollision->Activate();
	ProjectileMovement->Activate();
}

void AAct_Bullet::HitCheck(UPrimitiveComponent* HitComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent,FVector NormalImpulse,const FHitResult& Hit)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : localhit with (%s)"),*LINE_INFO,*GetNameSafe(this),*GetNameSafe(OtherActor));
		return;
	}

	UE_LOG(Proto,Warning,TEXT("%s / %s : server hit with (%s)"),*LINE_INFO,*GetNameSafe(this),*GetNameSafe(OtherActor));
	
	ProjectileMovement->StopMovementImmediately();
	UGameplayStatics::ApplyDamage(OtherActor,Damage,DamageInstigatorPlayer,this,UDamageType::StaticClass());

	CollisionMulticast(Hit.ImpactPoint);
	
}

void AAct_Bullet::BeginOverlap(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComponent,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult)
{
	
	if(GetLocalRole()<ROLE_Authority)
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : localOverLAP with (%s)"),*LINE_INFO,*GetNameSafe(this),*GetNameSafe(OtherActor));
		return;
	}


	UE_LOG(Proto,Warning,TEXT("%s / %s : serverOverLAP with (%s)"),*LINE_INFO,*GetNameSafe(this),*GetNameSafe(OtherActor));
	ProjectileMovement->StopMovementImmediately();

	UGameplayStatics::ApplyDamage(OtherActor,Damage,DamageInstigatorPlayer,this,UDamageType::StaticClass());
	

	//FVector DamageDirection = SweepResult.ImpactPoint - GetActorLocation();
	//UGameplayStatics::ApplyPointDamage(OtherActor, Damage, DamageDirection, SweepResult, DamageInstigatorPlayer, GetOwner(), UDamageType::StaticClass());

	CollisionMulticast(SweepResult.ImpactPoint);

	//ExplodeFX->OnSystemFinished.AddDynamic(this,&AAct_Bullet::StopFX);

}

bool AAct_Bullet::StopFX_Validate(UParticleSystemComponent* PSystem)
{
	return true;
}

void AAct_Bullet::StopFX_Implementation(UParticleSystemComponent* PSystem)
{
	Destroy();
}

void AAct_Bullet::CollisionMulticast_Implementation(FVector CollisionLocation)
{
	TracerFX->DeactivateSystem();
	SetActorEnableCollision(false);
	if(IsValid(ExplodeFX->Template))
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ExplodeFX->Template,FTransform(FRotator::ZeroRotator,CollisionLocation),true,EPSCPoolMethod::None,true);
	}	
	Destroy();
}



