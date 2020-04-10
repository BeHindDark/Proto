// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_DB_ProjectileWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"


void AAct_DB_ProjectileWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
		 
}
AAct_DB_ProjectileWeaponBase::AAct_DB_ProjectileWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	FireParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FireEffectObject"));
	EffectFire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireSystem"));

	Mesh->SetupAttachment(DefaultSceneRoot);
	Mesh->SetRelativeRotation(FRotator(90, -90, 0));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_Fire(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (P_Fire.Succeeded())
	{
		FireParticle = P_Fire.Object;
	}

}


void AAct_DB_ProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//블루프린트에 애님인스턴스 추가해야함!

	UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	if(IsValid(Anim)){
		Anim->UpFireCheck.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::UpFire);
		Anim->DownFireCheck.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::DownFire);
		Anim->AnimationEnd.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::AnimationEnd);
	}
	/** 이펙트를 지정해줍니다.
	*/



	
}

void AAct_DB_ProjectileWeaponBase::GetBlueprint(UArrowComponent* Arrow1, UArrowComponent* Arrow2, USceneComponent* EX_Muzzle_Location, USceneComponent* EX_Muzzle_Location2)
{
	FirstArrow = Arrow1;
	SecondArrow = Arrow2;
	ExMuzzle = EX_Muzzle_Location;
	
	ExMuzzle2 = EX_Muzzle_Location2;
}

void AAct_DB_ProjectileWeaponBase::UpFire()
{

	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	if (!IsValid(Anim)) {

		CHECK_LOG(!IsValid(Anim));
		return;
	}
	if (ProjectileClass)
	{

		//GameStatic->SpawnEmitterAttached(FireParticle, Mesh, FName("Barrel_End_1"));

		//Barrel_End_1
		FVector Front = FirstArrow->GetComponentLocation();
		FRotator Rotate = FirstArrow->GetComponentRotation();
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);
			
			if (FireParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, FireParticle, Front)->AttachToComponent(FirstArrow,FAttachmentTransformRules::KeepRelativeTransform);
				
			}

		}

	}

}
void AAct_DB_ProjectileWeaponBase::DownFire()
{
	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());

	if (!IsValid(Anim)) {
		CHECK_LOG(!IsValid(Anim));
		return;
	}
	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("if projectileClass is Execute"));
		FVector Front = SecondArrow->GetComponentLocation();
		FRotator Rotate = SecondArrow->GetComponentRotation();
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);
			if (FireParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, FireParticle, Front)->AttachToComponent(SecondArrow,FAttachmentTransformRules::KeepRelativeTransform );
				
				
			}
		}

	}

}
void AAct_DB_ProjectileWeaponBase::AnimationEnd()
{

}


void AAct_DB_ProjectileWeaponBase::Attack()
{
	
}

void AAct_DB_ProjectileWeaponBase::ServerOnFireOrder()
{
	Super::ServerOnFireOrder();
	
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	IsClicking = true;
}

void AAct_DB_ProjectileWeaponBase::ServerOnCeaseFireOrder()
{
	Super::ServerOnCeaseFireOrder();
	IsClicking = false;
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	
}
