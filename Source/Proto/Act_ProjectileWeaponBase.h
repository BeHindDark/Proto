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
	float LaunchSpeed = 20000.0f;

protected:
	UFUNCTION()
	void TurnTowardProjectileAim(float DeltaTime);
};
