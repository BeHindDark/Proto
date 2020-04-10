// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_DB_Weapon_AnimInstance.h"

#include "Act_ProjectileWeaponBase.h"


void UAnim_DB_Weapon_AnimInstance::AnimNotify_UpFireCheck()
{
	/** 위쪽 총열에서 총알이 나갈타이밍에 실행되는 함수입니다. 그타이밍에 브로드캐스트해줍니다.
	*/
	UpFireCheck.Broadcast();
}

void UAnim_DB_Weapon_AnimInstance::AnimNotify_DownFireCheck()
{
	/** 아래쪽 총열에서 총알이 나갈타이밍에 실행되는 함수입니다. 그타이밍에 브로드캐스트해줍니다.
	*/
	DownFireCheck.Broadcast();
}
void UAnim_DB_Weapon_AnimInstance::AnimNotify_AnimationEnd()
{
	AnimationEnd.Broadcast();
}