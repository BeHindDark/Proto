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

protected:
	UFUNCTION()
	/**	주어진 타겟에 투사체를 명중시킬 수 있도록 포탑을 회전시키는 함수입니다.
	*	LaunchSpeed, TargetLocation, 
	*	매 틱마다 호출됩니다.
	*/
	void TurnTowardProjectileAim(float DeltaTime);
};
