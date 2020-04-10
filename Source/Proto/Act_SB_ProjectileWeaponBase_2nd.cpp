// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_SB_ProjectileWeaponBase_2nd.h"
#include "WeaponControlSystem.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"

AAct_SB_ProjectileWeaponBase_2nd::AAct_SB_ProjectileWeaponBase_2nd()
{
}

void AAct_SB_ProjectileWeaponBase_2nd::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(WeaponSkeletalMesh->GetAnimInstance());
	if(IsValid(Anim))
	{
		Anim->UpFireCheck.AddDynamic(this,&AAct_SB_ProjectileWeaponBase_2nd::SBFire);
	}
}

void AAct_SB_ProjectileWeaponBase_2nd::BeginPlay()
{
	Super::BeginPlay();
}


void AAct_SB_ProjectileWeaponBase_2nd::SBFire()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	FVector MuzzleLocation = SingleMuzzleArrow->GetComponentLocation();
	FRotator LaunchDirection = GetActorRotation();
	SpawnBulletInServer(MuzzleLocation,LaunchDirection);
	MulticastFireFX(SingleMuzzleArrow);
}


void AAct_SB_ProjectileWeaponBase_2nd::ServerOnFireOrder()
{
	Super::ServerOnFireOrder();

	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	IsTriggerOn = true;
}

void AAct_SB_ProjectileWeaponBase_2nd::ServerOnCeaseFireOrder()
{
	Super::ServerOnCeaseFireOrder();
	if (GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	IsTriggerOn  = false;
}
