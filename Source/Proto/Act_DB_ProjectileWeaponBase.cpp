// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_DB_ProjectileWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"


AAct_DB_ProjectileWeaponBase::AAct_DB_ProjectileWeaponBase()
{

}

void AAct_DB_ProjectileWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();		

	UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(WeaponSkeletalMesh->GetAnimInstance());
	if(IsValid(Anim)){
		Anim->UpFireCheck.AddDynamic(this,&AAct_DB_ProjectileWeaponBase::UpFire);
		Anim->DownFireCheck.AddDynamic(this,&AAct_DB_ProjectileWeaponBase::DownFire);
		Anim->Weapon = this;
	}
	
}

void AAct_DB_ProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAct_DB_ProjectileWeaponBase::SetMuzzleArrows(UArrowComponent* Arrow1, UArrowComponent* Arrow2)
{
	UpperArrow = Arrow1;
	LowerArrow = Arrow2;
}

void AAct_DB_ProjectileWeaponBase::UpFire()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	FVector MuzzleLocation = UpperArrow->GetComponentLocation();
	FRotator LaunchDirection = GetActorRotation();
	SpawnBulletInServer(MuzzleLocation,LaunchDirection);
	MulticastFireFX(UpperArrow);
}
void AAct_DB_ProjectileWeaponBase::DownFire()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	FVector MuzzleLocation = LowerArrow->GetComponentLocation();
	FRotator LaunchDirection = GetActorRotation();
	SpawnBulletInServer(MuzzleLocation,LaunchDirection);
	MulticastFireFX(LowerArrow);
}


void AAct_DB_ProjectileWeaponBase::ServerOnFireOrder()
{
	Super::ServerOnFireOrder();
	
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	IsTriggerOn = true;
}

void AAct_DB_ProjectileWeaponBase::ServerOnCeaseFireOrder()
{
	Super::ServerOnCeaseFireOrder();

	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	IsTriggerOn = false;
}
