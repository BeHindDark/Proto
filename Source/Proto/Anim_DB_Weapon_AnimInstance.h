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
	/** 위에 포대 노티파이 델리게이트
	*/
	FUpFireCheckDelegate UpFireCheck;

	/** 아래 포대 노티파이 델리게이트
	*/
	FDownFireCheckDelegate DownFireCheck;

	/** 애니메이션 종료 델리게이트
	*/
	FAnimationEndDelegate AnimationEnd;

	//애니메이션 대상
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class AAct_ProjectileWeaponBase* Weapon;

	//Weapon 레퍼런스를 받으면 true로 바뀝니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWeaponConnected;

private:
	/** 상부총열발사 애니메이션 노티파이
	*/
	UFUNCTION()
	void AnimNotify_UpFireCheck();
	
	/** 하부총열발사 애니메이션 노티파이
	*/
	UFUNCTION()
	void AnimNotify_DownFireCheck();

	UFUNCTION()
	void AnimNotify_AnimationEnd();


};
