// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_DB_Weapon_AnimInstance.h"

void UAnim_DB_Weapon_AnimInstance::AnimNotify_UpFireCheck()
{
	UpFireCheck.Broadcast();
}

void UAnim_DB_Weapon_AnimInstance::AnimNotify_DownFireCheck()
{
	DownFireCheck.Broadcast();
}
