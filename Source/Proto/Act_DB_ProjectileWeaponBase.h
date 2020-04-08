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

	/** �����ѿ� ���� �޽�
	*/
	USkeletalMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** �����ѿ��� ���ѿ� ��� �ַο�
	*/
	UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** �����ѿ��� �Ʒ��ѿ� ��� �ַο�
	*/
	UArrowComponent* SecondArrow;


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	void GetBlueprint(UArrowComponent* Arrow1, UArrowComponent* Arrow2, USceneComponent* EX_Muzzle_Location, USceneComponent* EX_Muzzle_Location2);

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
		void AnimationEnd();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** ���ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�
	*/
	void UpFire();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** �Ʒ� �ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�.
	*/

	void DownFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // �߻�ü Ŭ������ �־��ش�.
	/** �߻�ü(�Ѿ�)�� Ŭ������ �޾ƿ��� �����Դϴ�. ������ ��������� �������Ʈ���� �������ݴϴ�.
	*/
	TSubclassOf<class AAct_Bullet> ProjectileClass;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** �ѱ�ȭ�� ����Ʈ�� �������� ��ƼŬ �ý����Դϴ�.
	*/
	class UParticleSystem* FireParticle;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** �ѱ�ȭ�� ����Ʈ�� �������� ��ƼŬ �ý����Դϴ�.
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
		/** �ѱ��� ��Ȯ�� ��ġ�� �޴� �� ������Ʈ�Դϴ�.
		*/
		USceneComponent* ExMuzzle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
		/** �ѱ��� ��Ȯ�� ��ġ�� �޴� �� ������Ʈ�Դϴ�.
		*/
		USceneComponent* ExMuzzle2;

};
