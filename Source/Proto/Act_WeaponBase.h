// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/Actor.h"
#include "Act_WeaponBase.generated.h"

UCLASS()
class PROTO_API AAct_WeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAct_WeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	public:
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Weapon")
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	class UArrowComponent* SingleMuzzleArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
	float MaxRange = 50000.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	float YawRotationSpeedMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	float PitchRotationSpeedMultiplier = 1.0f;

	int WeaponIndex;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float YawRotationSpeed = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float PitchRotationSpeed = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<float> YawRotationLimit = {-190.0f, +190.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TArray<float> PitchRotationLimit = {-10.0f, +30.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	TWeakObjectPtr<class UArrowComponent> SocketArrow_Ref;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|WeaponControlSystem")
	TWeakObjectPtr<class UWeaponControlSystem> WeaponControlSystem_Ref;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess =  true))
	bool bLocked;

public:
	virtual void ServerOnFireOrder();

	virtual void ServerOnCeaseFireOrder();

	void ConnectWeaponControlSystem(class UWeaponControlSystem* NewWeaponControlSystem, int NewWeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Custom|Weapon")
	virtual void SetSingleMuzzleArrow(class UArrowComponent* MuzzleArrow);


protected:
	virtual void TurnTowardDirectAim(float DeltaTime);
};
