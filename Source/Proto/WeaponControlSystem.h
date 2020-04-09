// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Components/ActorComponent.h"
#include "WeaponControlSystem.generated.h"

USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	FWeaponData()
	{
		SocketArrow_Ref = nullptr;
		GroupIndex = 0;
		Weapon = nullptr;
		YawRotationLimit ={-190.0f, 190.0f};
		PitchRotationLimit ={-10.0f, 30.0f};
	};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	TWeakObjectPtr<class UArrowComponent> SocketArrow_Ref;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	int GroupIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	class AAct_WeaponBase* Weapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	TArray<float> YawRotationLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	TArray<float> PitchRotationLimit;
};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTO_API UWeaponControlSystem : public UActorComponent
{
	GENERATED_BODY()


public:	
	// Sets default values for this component's properties
	UWeaponControlSystem();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


public:
	
	//FWeaponFireOrderDelegate OnFireOrder;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	FVector TargetWorldLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	TArray<FWeaponData> WeaponDataArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom|WeaponConrolSystem")
	bool IsTriggerOn = false;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Custom|WeaponControlSystem")
	int WeaponGroupSelector = 0;
	
	TArray<TArray<int>> WeaponGroupArray;
	
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void ActivateWeaponGroup(int WeaponGroupIndex);
	void ActivateWeaponGroup_Implementation(int WeaponGroupIndex);
	bool ActivateWeaponGroup_Validate(int WeaponGroupIndex);

	UFUNCTION(BlueprintCallable,Category = "Custom|WeaponControlSystem")
	/**	스폰되어 SocketArrow에 Attach까지 완료된 무기를 WCS과 연동시킵니다.
	*	@param	NewWeapon			새로 스폰된 Weapon객체에 대한 reference입니다.
	*	@param	WeaponIndex			무기와 장착된 소켓의 Index입니다.
	*	@param	WeaponGroupIndex	무장 선택에 사용될 WeaponGroup을 설정합니다.
	*/
	bool SyncNewWeapon(class AAct_WeaponBase* NewWeapon, int WeaponIndex, int WeaponGroupIndex);

	//void AsyncWeapon(int WeaponIndex);

	
	/**	이 WCS이 다룰 무기의 개수를 설정하고 이에 맞게 WeaponArray, SocketArrowArray의 크기를 초기화합니다.
	*	@param WeaponNum	이 WCS에 연결될 무기의 수입니다. 캐릭터로부터 받아옵니다.
	*	@detail	미리 TArray의 크기를 초기화 함으로서 약간의 성능 향상 가능.
	*/
	void InitializeWeaponNumber(int32 NewWeaponNum);

	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	/**	새로운 ArrowComponent를 SocketArrowArray에 추가합니다.
	*	@param	NewSocketArrow	새로 연결될 ArrowComponent에 대한 포인터입니다.
	*	@param	int WeaponIndex	해당 ArrowComponent가 가질 Index입니다.
	*	@return bool Success	성공하면 true, 실패하면 false를 반환합니다.
	*	@detail	만약 현재 넣을 수 있는 무기의 개수보다 큰 Index를 주면 false를 반환합ㄴ디ㅏ.
				SocketArrow의 포인터는 약포인터로 바꿔 저장합니다.
	*/
	bool SetWeaponSocket(class UArrowComponent* NewSocketArrow, int WeaponIndex, const TArray<float>& YawLimit, const TArray<float>& PitchLimit);
	
	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void SendFireOrder();

	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void SendCeaseFireOrder();
};