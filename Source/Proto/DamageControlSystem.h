// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Components/ActorComponent.h"
#include "DamageControlSystem.generated.h"

DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnHPIsZeroDelegate, class AController*, class AActor*, class AActor*);
DECLARE_MULTICAST_DELEGATE(FOnHPIsChangedDelegate);

/**	데미지를 주는 대상을 위젯이 구분할 수 있게 하기 위한 UENUM
*/
UENUM(BlueprintType)
enum class EDamageInstigatorType: uint8
{
	DamageInstigator_None			UMETA(DisplayName="None", ToolTip="특별한 정보가 없는 일반적인 데미지"),
	DamageInstigator_Enemy			UMETA(DisplayName="Enemy", ToolTip="적군 플레이어, 또는 AI가 가한 데미지"),
	DamageInstigator_Friendly		UMETA(DisplayName="Friendly", ToolTip="아군 사격에 의한 데미지"),
	DamageInstigator_Environmental	UMETA(DisplayName="Enviromental", ToolTip="환경요소에 의한 데미지")
};

/**	Spider와 같은 Pawn의 HP를 관리하고, 위젯등에 띄워주는 ActorComponent입니다.
*	Replication을 통해 데미지 적용 결과를 알립니다.
*	HP에 대한 버프 및 디버프 또한 관리합니다.(예정)
*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROTO_API UDamageControlSystem : public UActorComponent
{
	GENERATED_BODY()


public:
	// Sets default values for this component's properties
	UDamageControlSystem();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	/**	HP가 0이 되었을 때 Owner등에게 알리기 위한 Multicast Delegate
	*	EventInstigator로 Controller가 필요하며, 서버에서만 BroadCast
	*	@param	class AController*			EventInstigator
	*	@param	class AActor*				DamageCauser
	*	@param	class AActor*				DamageReciever
	*/
	FOnHPIsZeroDelegate OnHPIsZero;

	/**	HP에 변동이 있을 떄 각종 위젯 등에 업데이트를 지시하기 위한 Multicast Delegate
	*	각 위젯은 DamageControlSystem에 대한 레퍼런스를 가져야하며, GetHPRatio 등을 통해 알아서 업데이트 한다.
	*	Multicast ApplyDamage 함수에 의해 모든 클라이언트에서 BroadCast 됩니다.
	*/
	FOnHPIsChangedDelegate OnHPIsChanged;

private:
	//HP 초기화를 위한 기본 HP값입니다.
	UPROPERTY(EditDefaultsOnly, Category = "HP")
	float InitialMaxHP = 100.0f;

private:
	//현재의 HP입니다.
	UPROPERTY(VisibleAnywhere, Category = "HP", meta = (AllowPrivateAccess = true))
	float CurrentHP;
	
	//버프 디버프에 의해 변경된 현재 최대HP입니다.
	UPROPERTY(VisibleAnyWhere, Category = "HP", meta = (AllowPrivateAccess = true))
	float CurrentMaxHP;

public:
	float GetHPRatio();

	float GetCurrentMaxHP();

	float GetCurrentHP();

	/**	DamageControlSystem에 초기값을 설정합니다.
	*	Owner Actor의 PostInitializeComponents에서만 실행하십시오.
	*/
	void InitializeSystem(float MaxHP);

	/**	Owner Actor의 BeginPlay에서 호출됩니다.
	*	클라이언트가 액터 생성보다 늦게 접속했을 때를 위해, 각종 변수를 Sync시켜줍니다.
	*/
	void StartSystem();

	/**	데미지를 처리하고 적용하는 함수입니다.
	*	Actor의 TakeDamage함수안에서 호출되어 로직을 대신 실행해줍니다.
	*	만약 TeamShot이 꺼져있다면 Actor의 TakeDamage에서 이 함수를 호출하지 않아야 합니다.
	*/
	float OnTakeDamage(float Damage,struct FDamageEvent const& DamageEvent,class AController* EventInstigator,class AActor* DamageCauser);

private:
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_AfterDamageApplied(float Damage, float NewCurrentHP, float NewCurrentMaxHP, class AActor* DamageCauser, EDamageInstigatorType DamageInstigatorType, const FString& DamageInstigatorName);
	void Multicast_AfterDamageApplied_Implementation(float Damage, float NewCurrentHP, float NewCurrentMaxHP, class AActor* DamageCauser, EDamageInstigatorType DamageInstigatorType, const FString& DamageInstigatorName);

	UFUNCTION(Server, Reliable)
	void Server_RequestDataSync();
	void Server_RequestDataSync_Implementation();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_DataSync(float ServerHP, float ServerMaxHP);
	void Multicast_DataSync_Implementation(float ServerHP,float ServerMaxHP);
};
