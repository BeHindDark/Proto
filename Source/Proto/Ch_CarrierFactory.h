// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
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
	//void AttachMesh(FName SocketName, UStaticMeshComponent* MotherMeshName, UStaticMeshComponent* AttachMeshName, FName ComponentName);

	FVector CameraAimLocation(UCameraComponent* CurrentCamera);

	void Turn(float NewAxisValue);

	void LookUp(float NewAxisValue);

	void MoveForward(float NewAxisValue);

	void MoveRight(float NewAxisValue);

	void TurnBody(float NewAxisValue);

	float CameraPitchMovement;

	float CameraPitchSpeed;

	float CameraYawMovement;

	float CameraYawSpeed;

	float MoveInput = 0.0f;

	float BodyYawSpeed = 0.3;

	bool bIsPlayerControlling;

	FVector AimLocation;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "WeaponControlSystem")
	class UWeaponControlSystem* WCS;

	USpringArmComponent* SpringArm;

	UCameraComponent* Camera;

	FVector GetCameraAimLocation();
};
