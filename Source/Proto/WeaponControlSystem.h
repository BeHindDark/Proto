// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Components/ActorComponent.h"
#include "WeaponControlSystem.generated.h"

/**	무기와 소켓의 정보를 저장하기 위한 구조체 입니다.
*/
USTRUCT(BlueprintType)
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	FWeaponData():
		SocketArrow_Ref(nullptr),
		GroupIndex(0),
		Weapon(nullptr),
		YawRotationLimit(TArray<float>({-190.0f, 190.0f})),
		PitchRotationLimit(TArray<float>({-10.0f, 30.0f}))
	{};

	//무기가 장착될 소켓역할을 하는 ArrowComponent에 대한 약포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	TWeakObjectPtr<class UArrowComponent> SocketArrow_Ref;

	//현재 이 무기가 속해있는 무기선택그룹의 Index입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	int GroupIndex;

	//장착된 무기 액터에 대한 포인터입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	class AAct_WeaponBase* Weapon;

	//Yaw방향 회전각도 제한입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	TArray<float> YawRotationLimit;

	//Pitch방향 회전각도 제한입니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Socket")
	TArray<float> PitchRotationLimit;
};

/**	Int 2차원 TArray 배열을 만들기 위한 구조체입니다.
*/
USTRUCT(BlueprintType)
struct FIndexArray
{
	GENERATED_USTRUCT_BODY()

	FIndexArray():
		Array(TArray<int>())
	{};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "")
	TArray<int> Array;

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

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "ControlSystem")
	FVector TargetWorldLocation = FVector::ZeroVector;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "WeaponData")
	TArray<FWeaponData> WeaponDataArray;

	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "ConrolSystem")
	bool IsTriggerOn = false;

protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite, Category = "ControlSystem")
	int WeaponGroupSelector = 0;
	
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "ControlSystem")
	TArray<FIndexArray> WeaponGroupArray;
	
public:
	UFUNCTION(Server, Reliable, BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void ActivateWeaponGroup(int WeaponGroupIndex);
	void ActivateWeaponGroup_Implementation(int WeaponGroupIndex);

	/**	스폰된 무기를 SocketArrow에 Attach시키고 WCS과 연동시킵니다.
	*	서버에서만 실행시키면 client에선 알아서 replicate 됩니다.
	*	@param	NewWeapon			새로 스폰된 Weapon객체에 대한 reference입니다.
	*	@param	WeaponIndex			무기와 장착될 소켓의 Index입니다.
	*	@param	WeaponGroupIndex	무장 선택에 사용될 WeaponGroup을 설정합니다.
	*/
	UFUNCTION(BlueprintCallable,Category = "Custom|WeaponControlSystem")
	bool SyncNewWeapon(class AAct_WeaponBase* NewWeapon, int WeaponIndex, int WeaponGroupIndex);

	//void AsyncWeapon(int WeaponIndex);
		
	/**	이 WCS이 다룰 무기의 개수를 설정하고 이에 맞게 WeaponArray, SocketArrowArray의 크기를 초기화합니다.
	*	@param	WeaponNum	이 WCS에 연결될 무기의 수입니다. 캐릭터로부터 받아옵니다.
	*	@detail	미리 TArray의 크기를 초기화 함으로서 약간의 성능 향상 가능.
	*/
	void InitializeWeaponNumber(int32 NewWeaponNum);

	/**	새로운 ArrowComponent를 SocketArrowArray에 추가합니다.
	*	만약 현재 넣을 수 있는 무기의 개수보다 큰 Index를 주면 false를 반환합니다.
	*	SocketArrow의 포인터는 약포인터로 바꿔 저장합니다.
	*	@param	NewSocketArrow	새로 연결될 ArrowComponent에 대한 포인터입니다.
	*	@param	WeaponIndex		해당 ArrowComponent와 여기에 부착될 무기가 가질 Index입니다.
	*	@param	YawLimit		Yaw 방향의 회전각도 제한입니다. 크기가 2인 float 배열이며, [0]<0.0f<[1]이여야 합니다.
	*	@param	PitchLimit		Pitch 방향의 회전각도 제한입니다. 크기가 2인 float 배열이며, [0]<0.0f<[1]이여야 합니다.
	*	@return	Success			소켓 설정에 성공하면 true, 실패하면 false를 반환합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	bool SetWeaponSocket(class UArrowComponent* NewSocketArrow, int WeaponIndex, const TArray<float>& YawLimit, const TArray<float>& PitchLimit);
	
	/**	로컬에 있는 무기를 불러와 서버에서 발사하도록 합니다.
	*	클라이언트에서 실행되어야 합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void SendFireOrder();

	/**	로컬에 있는 무기를 불러와 서버에서 발사를 중지하도록 합니다.
	*	클라이언트에서 실행되어야 합니다.
	*/
	UFUNCTION(BlueprintCallable, Category = "Custom|WeaponControlSystem")
	void SendCeaseFireOrder();
};
