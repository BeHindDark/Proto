// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Act_ProjectileWeaponBase.h"
#include "Act_DB_ProjectileWeaponBase.generated.h"

/**
 *
 */
UCLASS()
class PROTO_API AAct_DB_ProjectileWeaponBase : public AAct_ProjectileWeaponBase
{
	GENERATED_BODY()

	
public:
	AAct_DB_ProjectileWeaponBase();

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** �����ѿ��� ���ѿ� ��� �ַο�
	*/
	UArrowComponent* UpperArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** �����ѿ��� �Ʒ��ѿ� ��� �ַο�
	*/
	UArrowComponent* LowerArrow;
		
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	void SetMuzzleArrows(UArrowComponent* Arrow1, UArrowComponent* Arrow2);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** ���ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�
	*/
	void UpFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** �Ʒ� �ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�.
	*/
	void DownFire();

	virtual void ServerOnFireOrder() override;

	virtual void ServerOnCeaseFireOrder() override;
};
