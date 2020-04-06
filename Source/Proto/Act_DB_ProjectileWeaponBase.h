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
<<<<<<< HEAD
	/** 이중총열 무기 메쉬
	*/
=======
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
	USkeletalMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
<<<<<<< HEAD
	/** 이중총열중 윗총열 담당 애로우
	*/
	UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** 이중총열중 아래총열 담당 애로우
	*/
	UArrowComponent* SecondArrow;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 블루프린트에서 지정한 애로우를 C++로 받아옵니다.
	*/
=======
	UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	UArrowComponent* SecondArrow;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
	void GetArrowComponent(UArrowComponent* Arrow1, UArrowComponent* Arrow2);


//	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	//	void SetAnimationDBWeapon(TSubclassOf<UAnimInstance> DB_Anim);


<<<<<<< HEAD
	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 윗총열에서 총알을 발사하는 함수입니다
	*/
	void UpFire();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** 아래 총열에서 총알을 발사하는 함수입니다.
	*/
=======
	void UpFire();

>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
	void DownFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // 발사체 클래스를 넣어준다.
	/** 발사체(총알)의 클래스를 받아오는 변수입니다. 지금은 비어있으나 블루프린트에서 지정해줍니다.
	*/
	TSubclassOf<class AAct_Bullet> ProjectileClass;

<<<<<<< HEAD
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
	*/
	class UParticleSystem* FireParticle;

<<<<<<< HEAD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** 이펙트를 Spawn하기위해 필요한 UGameplayStatics입니다.
	*/
	class UGameplayStatics* GameStatic;
=======
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // 발사체 클래스를 넣어준다.
	TSubclassOf<class AAct_Bullet> ProjectileClass;
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
};
