// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Ch_CarrierFactory.generated.h"

UCLASS()
class PROTO_API ACh_CarrierFactory : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_CarrierFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//TArray<WeaponIndexArray> WeaponControllIndex;

	TArray<TArray<int32>> WeaponArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Array")
	TArray<UArrowComponent*> ArrowArrayIndex;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ShoulderMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* CockpitMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LShoulderMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RShoulderMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LWeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RWeaponMesh = nullptr;
	

private:
	void AttachMesh(FName SocketName, UStaticMeshComponent* MotherMeshName, UStaticMeshComponent* AttachMeshName, FName ComponentName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
