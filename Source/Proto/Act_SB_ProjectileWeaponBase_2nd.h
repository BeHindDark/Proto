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

	virtual void BeginPlay() override;
	virtual void PostInitializeComponents() override;


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|SB")
		void GetArrowComponent_SB(UArrowComponent* Arrow1USceneComponent, USceneComponent* EX_Muzzle_Location);


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
		/** 총열에서 총알을 발사하는 함수입니다
		*/
	void UpFire();
	
	UPROPERTY(Category = "Actor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* Mesh;

	UPROPERTY(Category = "Weapon|Projectile|DB", VisibleAnywhere, BlueprintReadWrite)
		UArrowComponent* FirstArrow;

	UPROPERTY(Category = "Gameplay", EditAnywhere, BlueprintReadWrite)
		FVector MuzzleOffset;

	UPROPERTY(Category = "Projectile", EditDefaultsOnly, BlueprintReadOnly)  // 발사체 클래스를 넣어준다.
		TSubclassOf<class AAct_Bullet> ProjectileClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
		*/
		class UParticleSystem* FireParticle;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
		*/
		class UParticleSystemComponent* EffectFire;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** 총구의 정확한 위치를 받는 씬 컴포넌트입니다.
		*/
		USceneComponent* ExMuzzle;


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
		void AnimationEnd();


	UPROPERTY(Category = "Attack", VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		bool IsAttacking = false;

	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
		bool CanAttack = true;

	UPROPERTY(Category = "Attack", EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
		bool IsClicking = false;

};
