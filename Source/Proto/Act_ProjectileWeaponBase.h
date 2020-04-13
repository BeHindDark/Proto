// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Act_WeaponBase.h"
#include "Act_ProjectileWeaponBase.generated.h"


/**
 * 
 */
UCLASS()
class PROTO_API AAct_ProjectileWeaponBase : public AAct_WeaponBase
{
	GENERATED_BODY()

public:	
		
	// Sets default values for this actor's properties
	AAct_ProjectileWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	 
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Projectile")
	/**	�߻�ü�� �߻�Ǵ� �ӵ��Դϴ�.
	*	��ź�� ������ų�� �� ���� ���⺤�͸� ���ؼ� �ӵ��� ��� ��ǥ�� ����� �����մϴ�.
	*/
	float LaunchSpeed = 20000.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
	/**	������ ���ݹ�ư�� ������ �ִ��� ǥ���ϴ� �����Դϴ�.
	*	Animation���� ���˴ϴ�.
	*/
	bool IsTriggerOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon|Projectile|FX")
	FLinearColor TracerColor = FLinearColor(0.87f, 0.3f, 0.0f, 0.5f);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Weapon|Projectile")
	/** ����ü ������ ���̷�Ż �޽�
	*/
	USkeletalMeshComponent* WeaponSkeletalMesh;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile|FX")
	/** �ѱ�ȭ�� ����Ʈ�� �������� ��ƼŬ �ý����Դϴ�.
	*/
	class UParticleSystem* MuzzleFrameParticle;

protected:
	UFUNCTION()
	/**	�־��� Ÿ�ٿ� ����ü�� ���߽�ų �� �ֵ��� ��ž�� ȸ����Ű�� �Լ��Դϴ�.
	*	LaunchSpeed, TargetLocation�� ����� �����Ǿ� �־�� �մϴ�.
	*	�� ƽ���� ȣ��˴ϴ�.
	*	���� ���ߺҰ����� ��ġ��� �ش� ������ �ȹٷ� �ٶ󺾴ϴ�.
	*/
	void TurnTowardProjectileAim(float DeltaTime);

	/**	���������� ����Ǵ� �Լ��Դϴ�.
	*	�ѱ���ġ���� ���ϴ� �������� �Ѿ��� ������Ű�� �����ϴ�.
	*	@param	MuzzleLocation	�Ѿ��� ������ ��ġ
	*	@param	LaunchRotation	�Ѿ��� �߻�� ����
	*	�߻������ ArrowComponent�� ������ �� �� �� �ְ�, ���Ͱ� �ٶ󺸴� ������ �� ���� �ֽ��ϴ�.
	*/
	void SpawnBulletInServer(FVector MuzzleLocation,FRotator LaunchRotation);

	UFUNCTION(NetMulticast, Unreliable, Category = "Custom|Weapon|Projectile|FX")
	/**	�������� ��Ƽĳ��Ʈ�ϴ� �Լ��Դϴ�.
	*	�ѱ���ġ�� �˷��ִ� ArrowComponent�� �̿��Ͽ� �ش� ��ġ�� ���� ����Ʈ�� ������Ű�� Attach��ŵ�ϴ�.
	*/
	void MulticastFireFX(class UArrowComponent* MuzzleArrow);
	void MulticastFireFX_Implementation(class UArrowComponent* MuzzleArrow);
};
