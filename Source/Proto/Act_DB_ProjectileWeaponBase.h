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
	/** �����ѿ� ���� �޽�
	*/
=======
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
	USkeletalMeshComponent* Mesh;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
<<<<<<< HEAD
	/** �����ѿ��� ���ѿ� ��� �ַο�
	*/
	UArrowComponent* FirstArrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile|DB")
	/** �����ѿ��� �Ʒ��ѿ� ��� �ַο�
	*/
	UArrowComponent* SecondArrow;

	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** �������Ʈ���� ������ �ַο츦 C++�� �޾ƿɴϴ�.
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
	/** ���ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�
	*/
	void UpFire();


	UFUNCTION(BlueprintCallable, Category = "Weapon|Projectile|DB")
	/** �Ʒ� �ѿ����� �Ѿ��� �߻��ϴ� �Լ��Դϴ�.
	*/
=======
	void UpFire();

>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
	void DownFire();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // �߻�ü Ŭ������ �־��ش�.
	/** �߻�ü(�Ѿ�)�� Ŭ������ �޾ƿ��� �����Դϴ�. ������ ��������� �������Ʈ���� �������ݴϴ�.
	*/
	TSubclassOf<class AAct_Bullet> ProjectileClass;

<<<<<<< HEAD
=======
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	FVector MuzzleOffset;
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** �ѱ�ȭ�� ����Ʈ�� �������� ��ƼŬ �ý����Դϴ�.
	*/
	class UParticleSystem* FireParticle;

<<<<<<< HEAD
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)
	/** ����Ʈ�� Spawn�ϱ����� �ʿ��� UGameplayStatics�Դϴ�.
	*/
	class UGameplayStatics* GameStatic;
=======
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Projectile)  // �߻�ü Ŭ������ �־��ش�.
	TSubclassOf<class AAct_Bullet> ProjectileClass;
>>>>>>> 7a04dff6867e7a84f85fe7674de83500d80de2b1
};
