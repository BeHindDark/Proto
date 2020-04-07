// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_DB_Weapon_AnimInstance.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpFireCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDownFireCheckDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAnimationEndDelegate);

/**
 *
 */
UCLASS()
class PROTO_API UAnim_DB_Weapon_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	/** ���� ���� ��Ƽ���� ��������Ʈ
	*/
	FUpFireCheckDelegate UpFireCheck;
	/** �Ʒ� ���� ��Ƽ���� ��������Ʈ
	*/
	FDownFireCheckDelegate DownFireCheck;
	/** �ִϸ��̼� ���� ��������Ʈ
	*/
	FAnimationEndDelegate AnimationEnd;


private:
	UFUNCTION()
	/** ���� ���� �ִϸ��̼� ��Ƽ����
	*/
	void AnimNotify_UpFireCheck();
	
	UFUNCTION()
	/** �Ʒ� ���� �ִϸ��̼� ��Ƽ����
	*/
	void AnimNotify_DownFireCheck();

	UFUNCTION()
	void AnimNotify_AnimationEnd();


};
