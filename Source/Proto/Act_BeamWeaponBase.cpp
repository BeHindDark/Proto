// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_BeamWeaponBase.h"

AAct_BeamWeaponBase::AAct_BeamWeaponBase()
{

}

void AAct_BeamWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAct_BeamWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(GetLocalRole()>=ROLE_Authority)
	{
		TurnTowardDirectAim(DeltaTime);
	}
}
