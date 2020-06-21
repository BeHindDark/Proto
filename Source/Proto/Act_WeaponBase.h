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
	//��ƽ���� WeaponControlSystem���κ��� ���޹��� ������ġ�� �����صδ� �����Դϴ�.
	FVector TargetLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	/**	źȯ�̳� ����Ʈ�� ������ ������ ������ġ�� �˷��ִ� Arrow�Դϴ�.
	*	C++�� �ƴ� �������Ʈ���� ������ ��
	*	SetSingleMuzzleArrow �Լ��� ���� �����͸� �޽��ϴ�.
	*/
	class UArrowComponent* SingleMuzzleArrow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon|Attack")
	//������ �ִ� �����Ÿ��Դϴ�. ����μ� ���̰��Դϴ�.
	float MaxRange = 50000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Attack")
	//������ �⺻ �������� �Դϴ�.
	float Damage = 10.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon|Attack")
	//���� �������� ���� �� ������� �ֱ� ���� ����Դϴ�.
	float DamageMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	//��ž�� Yaw���� ȸ���ӵ��� ����/������� �� ���� ����Դϴ�.
	float YawRotationSpeedMultiplier = 1.0f;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Weapon")
	//��ž�� Pitch���� ȸ���ӵ��� ����/������� �� ���� ����Դϴ�.
	float PitchRotationSpeedMultiplier = 1.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	/**	WeaponControlSystem�� WeaponDataArray���� �� ���Ⱑ �ִ� ��ġ�� �˷��ִ� Index�Դϴ�.
	*/
	int WeaponIndex = -1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	/**	������ ���⸦ �ٷ�� PlayerController, �Ǵ� AIController�Դϴ�.
	*	Controller�� OwningPawn�� Possess�� �� �� ���� �����մϴ�.
	*/
	AController* InstigatorController = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon|WeaponControlSystem")
	AActor* OwningActor = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess = true))
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//��ž�� Yaw���� ȸ���ӵ��Դϴ�.
	float YawRotationSpeed = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	//��ž�� Pitch���� ȸ���ӵ��Դϴ�.
	float PitchRotationSpeed = 30.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	/**	��ž�� Yaw���� ȸ�� ���� �����Դϴ�.
	*	����μ��� WeaponControlSystem�� ����ɶ�, �����ʿ� ����Ǿ� �ִ� ���� �׻� ��������ϴ�.
	*	������ ���� ������� �ʰ� ��ü���� default ���� ���� �ɼǵ� ���� �����Դϴ�.
	*/
	TArray<float> YawRotationLimit = {-190.0f, +190.0f};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	/**	��ž�� Pitch���� ȸ�� ���� �����Դϴ�.
	*	����μ��� WeaponControlSystem�� ����ɶ�, �����ʿ� ����Ǿ� �ִ� ���� �׻� ��������ϴ�.
	*	������ ���� ������� �ʰ� ��ü���� default ���� ���� �ɼǵ� ���� �����Դϴ�.
	*/
	TArray<float> PitchRotationLimit = {-10.0f, +30.0f};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
	/**	�� ���Ⱑ Attach�� ArrowComponent�� ���� ���������Դϴ�.
	*	���� ȸ���� �Ҷ� �������� ����ϴ�.
	*/
	TWeakObjectPtr<class UArrowComponent> SocketArrow_Ref;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon|WeaponControlSystem")
	/**	�� ���⸦ �����ϴ� WeaponControlSystem�� ���� ���������Դϴ�.
	*	���ظ�ǥ������ �޾ƿ� �� ���Դϴ�.
	*/
	class UWeaponControlSystem* WeaponControlSystem_Ref;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess =  true))
	//��ž�� Ÿ�ٿ� ���µǰų�, �ִ밢���� �ɷ� �ȿ����̴� ��Ȳ�̸� true
	bool bLocked;

	UPROPERTY(VisibleAnywhere, Category = "Weapon", meta = (AllowPrivateAccess =  true))
	//��ž�� Ÿ�ٿ� ���µ� �����ΰ�
	bool bIsOnTarget;

public:
	/**	���� ������ ���ݹ�ư(���콺��)�� ������ ���������� ����� �Լ��Դϴ�.
	*	WCS���� ȣ���ŵ�ϴ�.
	*	���⼭ ���� �ִϸ��̼� ���� ����˴ϴ�.
	*/
	virtual void ServerOnFireOrder();

	/**	���� ������ ���ݹ�ư(���콺��)�� ���� ���������� ����� �Լ��Դϴ�.
	*	���⼭ ���� �ִϸ��̼� ���� �����˴ϴ�.
	*/
	virtual void ServerOnCeaseFireOrder();

	/**	WeaponControlSystem�� �����մϴ�.
	*	�� �Լ��� �����ϱ� ���� WCS���� Socket������ ���� �߾�� �մϴ�.
	*	�������� ��� ������ ArrowComponent��  Attach�˴ϴ�.
	*/
	UFUNCTION(NetMulticast,Reliable)
	void ConnectWeaponControlSystem(class UWeaponControlSystem* NewWeaponControlSystem, int NewWeaponIndex);
	void ConnectWeaponControlSystem_Implementation(class UWeaponControlSystem* NewWeaponControlSystem,int NewWeaponIndex);

	UFUNCTION(BlueprintCallable, Category = "Custom|Weapon")
	/**	����Ʈ �� źȯ���� ������ų �ѱ�ArrowComponent�� ����մϴ�.
	*	�ڼ� �������Ʈ Ŭ������ BeginPlay���� �� ����Ǿ�� �մϴ�.
	*	@param MuzzleArrow	��Ͻ�ų ArrowComponent�� ������
	*/
	virtual void SetSingleMuzzleArrow(class UArrowComponent* MuzzleArrow);

	UFUNCTION(BlueprintCallable, Category = "Custom|Weapon")
	class UWeaponControlSystem* GetWeaponControlSystem();

protected:
	/**	�������� ���� Target�������� ��ž�� ȸ����Ű�� �Լ��Դϴ�.
	*	�� ƽ���� ȣ��˴ϴ�.
	*/
	virtual void TurnTowardDirectAim(float DeltaTime);
};
