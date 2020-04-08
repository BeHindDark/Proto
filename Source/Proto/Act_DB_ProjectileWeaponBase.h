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


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Weapon|Projectile|DB")

	/** 이중총열 무기 메쉬
	*/
	USkeletalMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** 이중총열중 윗총열 담당 애로우
	*/
	UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** 이중총열중 아래총열 담당 애로우
	*/
	UArrowComponent* SecondArrow;


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	void GetBlueprint(UArrowComponent* Arrow1, UArrowComponent* Arrow2, USceneComponent* EX_Muzzle_Location, USceneComponent* EX_Muzzle_Location2);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
		void AnimationEnd();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 윗총열에서 총알을 발사하는 함수입니다
	*/
	void UpFire();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 아래 총열에서 총알을 발사하는 함수입니다.
	*/

	void DownFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // 발사체 클래스를 넣어준다.
	/** 발사체(총알)의 클래스를 받아오는 변수입니다. 지금은 비어있으나 블루프린트에서 지정해줍니다.
	*/
	TSubclassOf<class AAct_Bullet> ProjectileClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
	*/
	class UParticleSystem* FireParticle;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
		*/
		class UParticleSystemComponent* EffectFire;

	UFUNCTION()

	void Attack();

	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
		bool IsAttacking = false;

	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
		bool CanAttack = true;

	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
		bool IsClicking = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구의 정확한 위치를 받는 씬 컴포넌트입니다.
		*/
		USceneComponent* ExMuzzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구의 정확한 위치를 받는 씬 컴포넌트입니다.
		*/
		USceneComponent* ExMuzzle2;

};
