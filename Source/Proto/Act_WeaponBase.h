// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/Actor.h"
#include "Act_WeaponBase.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FWeaponTakeDamageDelegate, float, DamageAmount, struct FDamageEvent const&, DamageEvent, class AController*, EventInstigator, AActor*, DamageCouser);



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

	virtual float TakeDamage(float DamageAmount,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,AActor* DamageCauser) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
public:

	FWeaponTakeDamageDelegate OnWeaponTakeDamage;

	UPROPERTY(Replicated, VisibleAnywhere, Category = "Weapon")
	//매틱마다 WeaponControlSystem으로부터 전달받은 조준위치를 저장해두는 변수입니다.
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	/**	탄환이나 이펙트가 생성될 무기의 포구위치를 알려주는 Arrow입니다.
	*	C++이 아닌 블루프린트에서 생성된 뒤
	*	SetSingleMuzzleArrow 함수를 통해 포인터를 받습니다.
	*/
	class UArrowComponent* SingleMuzzleArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Attack")
	//무기의 최대 사정거리입니다. 현재로선 더미값입니다.
	float MaxRange = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	//무기의 기본 데미지량 입니다.
	float Damage = 10.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon|Attack")
	//무기 데미지에 버프 및 디버프를 주기 위한 배수입니다.
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	//포탑의 Yaw방향 회전속도를 버프/디버프할 때 쓰는 배수입니다.
	float YawRotationSpeedMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	//포탑의 Pitch방향 회전속도를 버프/디버프할 때 쓰는 배수입니다.
	float PitchRotationSpeedMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	/**	WeaponControlSystem의 WeaponDataArray에서 이 무기가 있는 위치를 알려주는 Index입니다.
	*/
	int WeaponIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	/**	실제로 무기를 다루는 PlayerController, 또는 AIController입니다.
	*	Controller가 OwningPawn에 Possess할 될 떄 값을 전달합니다.
	*/
	AController* InstigatorController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	AActor* OwningActor = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//포탑의 Yaw방향 회전속도입니다.
	float YawRotationSpeed = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//포탑의 Pitch방향 회전속도입니다.
	float PitchRotationSpeed = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	/**	포탑의 Yaw방향 회전 각도 제한입니다.
	*	현재로서는 WeaponControlSystem에 연결될때, 소켓쪽에 저장되어 있는 값이 항상 덮어씌워집니다.
	*	하지만 후일 덮어씌우지 않고 자체적인 default 값을 쓰는 옵션도 넣을 예정입니다.
	*/
	TArray<float> YawRotationLimit = {-190.0f, +190.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	/**	포탑의 Pitch방향 회전 각도 제한입니다.
	*	현재로서는 WeaponControlSystem에 연결될때, 소켓쪽에 저장되어 있는 값이 항상 덮어씌워집니다.
	*	하지만 후일 덮어씌우지 않고 자체적인 default 값을 쓰는 옵션도 넣을 예정입니다.
	*/
	TArray<float> PitchRotationLimit = {-10.0f, +30.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	/**	이 무기가 Attach된 ArrowComponent에 대한 약포인터입니다.
	*	무기 회전을 할때 기준으로 삼습니다.
	*/
	TWeakObjectPtr<class UArrowComponent> SocketArrow_Ref;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|WeaponControlSystem")
	/**	이 무기를 관리하는 WeaponControlSystem에 대한 약포인터입니다.
	*	조준목표지점을 받아올 때 쓰입니다.
	*/
	class UWeaponControlSystem* WeaponControlSystem_Ref;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess =  true))
	//포탑이 타겟에 락온되거나, 최대각도에 걸려 안움직이는 상황이면 true
	bool bLocked;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess =  true))
	//포탑이 타겟에 락온된 상태인가
	bool bIsOnTarget;

public:
	/**	무기 주인이 공격버튼(마우스등)을 누르면 서버에서만 실행될 함수입니다.
	*	WCS에서 호출시킵니다.
	*	여기서 무기 애니매이션 등이 실행됩니다.
	*/
	virtual void ServerOnFireOrder();

	/**	무기 주인이 공격버튼(마우스등)을 떼면 서버에서만 실행될 함수입니다.
	*	여기서 무기 애니메이션 등이 중지됩니다.
	*/
	virtual void ServerOnCeaseFireOrder();

	/**	WeaponControlSystem과 연결합니다.
	*	이 함수를 실행하기 전에 WCS에서 Socket설정을 먼저 했어야 합니다.
	*	정상적인 경우 설정된 ArrowComponent에  Attach됩니다.
	*/
	UFUNCTION(NetMulticast,Reliable)
	void ConnectWeaponControlSystem(class UWeaponControlSystem* NewWeaponControlSystem, int NewWeaponIndex);
	void ConnectWeaponControlSystem_Implementation(class UWeaponControlSystem* NewWeaponControlSystem,int NewWeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Custom|Weapon")
	/**	이펙트 및 탄환등을 스폰시킬 총구ArrowComponent를 등록합니다.
	*	자손 블루프린트 클래스의 BeginPlay에서 꼭 실행되어야 합니다.
	*	@param MuzzleArrow	등록시킬 ArrowComponent의 포인터
	*/
	virtual void SetSingleMuzzleArrow(class UArrowComponent* MuzzleArrow);

	UFUNCTION(BlueprintCallable, Category = "Custom|Weapon")
	class UWeaponControlSystem* GetWeaponControlSystem();

protected:
	/**	직사사격을 위해 Target방향으로 포탑을 회전시키는 함수입니다.
	*	매 틱마다 호출됩니다.
	*/
	virtual void TurnTowardDirectAim(float DeltaTime);
};
