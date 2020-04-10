// Fill out your copyright notice in the Description page of Project Settings.


#include "Anim_DB_Weapon_AnimInstance.h"

#include "Act_ProjectileWeaponBase.h"


void UAnim_DB_Weapon_AnimInstance::AnimNotify_UpFireCheck()
{
	/** ���� �ѿ����� �Ѿ��� ����Ÿ�ֿ̹� ����Ǵ� �Լ��Դϴ�. ��Ÿ�ֿ̹� ��ε�ĳ��Ʈ���ݴϴ�.
	*/
	UpFireCheck.Broadcast();
}

void UAnim_DB_Weapon_AnimInstance::AnimNotify_DownFireCheck()
{
	/** �Ʒ��� �ѿ����� �Ѿ��� ����Ÿ�ֿ̹� ����Ǵ� �Լ��Դϴ�. ��Ÿ�ֿ̹� ��ε�ĳ��Ʈ���ݴϴ�.
	*/
	DownFireCheck.Broadcast();
}
void UAnim_DB_Weapon_AnimInstance::AnimNotify_AnimationEnd()
{
	AnimationEnd.Broadcast();
}