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
	/** 이중총열중 윗총열 담당 애로우
	*/
	UArrowComponent* UpperArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** 이중총열중 아래총열 담당 애로우
	*/
	UArrowComponent* LowerArrow;
		
	
public:
	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	void SetMuzzleArrows(UArrowComponent* Arrow1, UArrowComponent* Arrow2);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 윗총열에서 총알을 발사하는 함수입니다
	*/
	void UpFire();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 아래 총열에서 총알을 발사하는 함수입니다.
	*/
	void DownFire();

	virtual void ServerOnFireOrder() override;

	virtual void ServerOnCeaseFireOrder() override;
};
