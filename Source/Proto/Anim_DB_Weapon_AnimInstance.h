// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Anim_DB_Weapon_AnimInstance.generated.h"


DECLARE_MULTICAST_DELEGATE(FUpFireCheckDelegate)
DECLARE_MULTICAST_DELEGATE(FDownFireCheckDelegate)

/**
 *
 */
UCLASS()
class PROTO_API UAnim_DB_Weapon_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	FUpFireCheckDelegate UpFireCheck;
	FDownFireCheckDelegate DownFireCheck;
private:
	UFUNCTION()
	void AnimNotify_UpFireCheck();

	UFUNCTION()
	void AnimNotify_DownFireCheck();

};
