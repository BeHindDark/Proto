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
	/**	발사체가 발사되는 속도입니다.
	*	포탄을 스폰시킬때 이 값과 방향벡터를 곱해서 속도를 줘야 목표에 제대로 명중합니다.
	*/
	float LaunchSpeed = 20000.0f;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|Projectile")
	/**	유저가 공격버튼을 누르고 있는지 표시하는 변수입니다.
	*	Animation에서 사용됩니다.
	*/
	bool IsTriggerOn = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Weapon|Projectile|FX")
	FLinearColor TracerColor = FLinearColor(0.87f, 0.3f, 0.0f, 0.5f);


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category = "Weapon|Projectile")
	/** 투사체 무기의 스켈레탈 메쉬
	*/
	USkeletalMeshComponent* WeaponSkeletalMesh;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon|Projectile|FX")
	/** 총구화염 이펙트를 저장해줄 파티클 시스템입니다.
	*/
	class UParticleSystem* MuzzleFrameParticle;

protected:
	UFUNCTION()
	/**	주어진 타겟에 투사체를 명중시킬 수 있도록 포탑을 회전시키는 함수입니다.
	*	LaunchSpeed, TargetLocation이 제대로 설정되어 있어야 합니다.
	*	매 틱마다 호출됩니다.
	*	만약 명중불가능한 위치라면 해당 방향을 똑바로 바라봅니다.
	*/
	void TurnTowardProjectileAim(float DeltaTime);

	/**	서버에서만 실행되는 함수입니다.
	*	총구위치에서 원하는 방향으로 총알을 스폰시키고 날립니다.
	*	@param	MuzzleLocation	총알이 스폰될 위치
	*	@param	LaunchRotation	총알이 발사될 방향
	*	발사방향은 ArrowComponent의 방향이 될 수 도 있고, 액터가 바라보는 방향을 쓸 수도 있습니다.
	*/
	void SpawnBulletInServer(FVector MuzzleLocation,FRotator LaunchRotation);

	UFUNCTION(NetMulticast, Unreliable, Category = "Custom|Weapon|Projectile|FX")
	/**	서버에서 멀티캐스트하는 함수입니다.
	*	총구위치를 알려주는 ArrowComponent를 이용하여 해당 위치에 각종 이펙트를 스폰시키고 Attach시킵니다.
	*/
	void MulticastFireFX(class UArrowComponent* MuzzleArrow);
	void MulticastFireFX_Implementation(class UArrowComponent* MuzzleArrow);
};
