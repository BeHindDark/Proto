// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Act_WeaponBase.h"
#include "Act_ProjectileWeaponBase.h"
#include "Act_SBProjectileWeaponBase.generated.h"


UCLASS()
class PROTO_API AAct_SBProjectileWeaponBase : public AActor
{
	GENERATED_BODY()
			
public:	
	// Sets default values for this actor's properties
	AAct_SBProjectileWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

protected:
	UPROPERTY(Category = "Actor", VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

private:
	UPROPERTY(Category="Attack", VisibleInstanceOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsAttacking;
};
