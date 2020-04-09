// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
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

	virtual void PossessedBy(AController* NewController) override;

	virtual void PostInitializeComponents() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spider|Movement")
	float MoveInput = 0.0f;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Spider|WeaponControlSystem")
	class UWeaponControlSystem* WCS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintreadWrite, Category = "Spider|Body")
	USceneComponent* WaistSceneComponent;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category = "Spider|Camera")
	float CameraPitchMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider|Camera")
	float CameraPitchSpeed = 60.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spider|Camera")
	float CameraYawMovement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly,Category = "Spider|Camera")
	float CameraYawSpeed  = 120.0f;;




	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spider|Movement")
	float BodyYawSpeed = 0.3;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spider|WeaponControlSystem")
	bool bIsPlayerControlling = false;

	UPROPERTY(replicated,VisibleAnywhere, BlueprintReadWrite, Category = "Spider|WeaponControlSystem")
	FVector AimLocation = FVector::ZeroVector;

	APlayerController* PlayerController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider|Movement")
	float UpperBodyRotationSpeed = 140.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spider|WeaponControlSystem")
	//���⸦ ���������� ������ ���Դϴ�. �������Ʈ���� �ʱⰪ�� �� ������Ѿ� �մϴ�.
	int32 WeaponSlotNum = 0;

public:
	UFUNCTION(Server,Reliable,WithValidation)
	void ServerNetTick(FVector CameraAim,float Deltatime);
	void ServerNetTick_Implementation(FVector CameraAim,float Deltatime);
	bool ServerNetTick_Validate(FVector CameraAim,float Deltatime);

protected:
	FVector CameraAimLocation(UCameraComponent* CurrentCamera);

	void Turn(float NewAxisValue);

	void LookUp(float NewAxisValue);

	void MoveForward(float NewAxisValue);

	void MoveRight(float NewAxisValue);

	void TurnBody(float NewAxisValue);

	UFUNCTION()
	void ChangeWeaponGroup(int NewGroup);

	UFUNCTION(Server, Reliable, WithValidation)
	void OnTriggerDown();
	void OnTriggerDown_Implementation();
	bool OnTriggerDown_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void OnTriggerUp();
	void OnTriggerUp_Implementation();
	bool OnTriggerUp_Validate();

	UFUNCTION(BlueprintCallable)
	void SetWaistSceneComponent(USceneComponent* BlueprintWaistSceneComponent);

private:
	void TurnUpperBody(USceneComponent* WaistComponent, float DeltaTime);
};
