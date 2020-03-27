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
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Arrow")
	UArrowComponent* CockpitArrow;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ShoulderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* CockpitMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LShoulderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RShoulderMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* LWeaponMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* RWeaponMesh;
	

private:

	void AttachShoulder(FName SocketName);

	void AttachLShoulder(FName SocketName);

	void AttachRShoulder(FName SocketName);

	void AttachCockpit(FName SocketName);

	void AttachLWeapon(FName SocketName);

	void AttachRWeapon(FName SocketName);

	void AttachMesh(FName SocketName, UStaticMeshComponent* MotherMeshName, UStaticMeshComponent* AttachMeshName, FName ComponentName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
