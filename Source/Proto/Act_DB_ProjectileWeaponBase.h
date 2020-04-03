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

private:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;
public:
	AAct_DB_ProjectileWeaponBase();

	UPROPERTY(VisibleAnywhere, Category = "Weapon|Projectile|DB")
		USkeletalMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
		UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
		UArrowComponent* SecondArrow;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
		void GetArrowComponent(UArrowComponent* Arrow1, UArrowComponent* Arrow2);


//	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	//	void SetAnimationDBWeapon(TSubclassOf<UAnimInstance> DB_Anim);


	void UpFire();

	void DownFire();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // 발사체 클래스를 넣어준다.
		TSubclassOf<class AAct_Bullet> ProjectileClass;
};
