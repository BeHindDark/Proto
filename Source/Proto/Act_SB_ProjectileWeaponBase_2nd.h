// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Proto.h"
#include "Act_WeaponBase.h"
#include "Act_ProjectileWeaponBase.h"
#include "Act_SB_ProjectileWeaponBase_2nd.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API AAct_SB_ProjectileWeaponBase_2nd : public AAct_ProjectileWeaponBase
{
	GENERATED_BODY()
	

public:
	AAct_SB_ProjectileWeaponBase_2nd();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|SB")
	/** �ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�
	*/
	void SBFire();
	

protected:
	virtual void ServerOnFireOrder() override;

	virtual void ServerOnCeaseFireOrder() override;
};
