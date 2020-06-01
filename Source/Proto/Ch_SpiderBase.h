// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/Character.h"
#include "Ch_SpiderBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpiderOnDeath,AController*,EventInstigator);


UCLASS()
class PROTO_API ACh_SpiderBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACh_SpiderBase();


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
	

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spider|Movement")
	float MoveInput = 0.0f;

	UPROPERTY(BlueprintAssignable,VisibleAnywhere,BlueprintCallable,Category = "Event")
	FSpiderOnDeath OnDeath;

protected:

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category = "Spider|WeaponControl")
	class UWeaponControlSystem* WeaponControlSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|DamageControl")
	class UDamageControlSystem* DamageControlSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|Camera")
	USpringArmComponent* SpringArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|Camera")
	UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spider|Body")
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

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "Spider|Player")
	bool bIsPlayerControlling = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spider|WeaponControl")
	FVector AimLocation = FVector::ZeroVector;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite,Category = "Spider|Player")
	TWeakObjectPtr<APlayerController> PlayerController;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spider|Movement")
	float UpperBodyRotationSpeed = 140.0f;

	//무기를 장착가능한 슬롯의 수입니다. 블루프린트에서 초기값을 꼭 변경시켜야 합니다.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spider|WeaponControl")
	int32 WeaponSlotNum = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HP")
	float CurrentHP;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HP")
	float MaxHP = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spider|Animation")
	UAnimationAsset* DeathAnimAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Network")
	bool bIsLocalPlayerControlled = false;

private:
	//매 틱마다 누적된 시간을 저장하는 변수
	float TickTimeStack = 0.0f;

	//네트워크 상에서 반복적으로 replicate되는 reliable함수의 실행간격 조절 변수
	float NetworkTickInterval = 0.05f;

public:

	UFUNCTION()
	void OnWeaponTakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) ;

	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) override;

protected:
	FVector GetCameraAimLocation(UCameraComponent* CurrentCamera);

	void TurnCamera(float NewAxisValue);

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

	/**	블루프린트에서 만든 상체 SceneComponent의 포인터를 C++로 전달합니다.
	*	BeginPlay에서 모든 클라이언트가 실행해야 합니다.
	*/
	UFUNCTION(BlueprintCallable)
	void SetWaistSceneComponent(USceneComponent* BlueprintWaistSceneComponent);

	UFUNCTION(NetMulticast, Reliable)
	void DeathAnim();
	void DeathAnim_Implementation();

	/**	NetworkTickInterval 변수에 의해 작동주기가 늦춰지는 네트워크 전용 Tick함수입니다.
	*	주기적으로 반복실행되어야하는 Reliable RPC함수를 실행시키는데 쓰입니다.
	*/
	virtual void NetworkTick(float NetworkDeltaTime);

	void OnHPIsZero(AController* DamageInstigator, AActor* DamageCauser, AActor* DamageReciever);

private:

	UFUNCTION(Server, Reliable)
	void ServerNetTick(FVector LocalAim);
	void ServerNetTick_Implementation(FVector LocalAim);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastNetTick(FVector ServerAim);
	void MulticastNetTick_Implementation(FVector ServerAim);

	void TurnUpperBody(USceneComponent* WaistComponent, float DeltaTime);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_SetUpperBodyYaw(float NewYaw);
	void Multicast_SetUpperBodyYaw_Implementation(float NewYaw);
};
