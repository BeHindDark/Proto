// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponControlSystem.h"
#include "BaseWeapon.h"
#include "Components/ArrowComponent.h"

// Sets default values for this component's properties
UWeaponControlSystem::UWeaponControlSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponControlSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UWeaponControlSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UWeaponControlSystem::SetActiveWeaponGroup(int WeaponGroupIndex)
{
	if((WeaponGroupIndex>=0)&&(WeaponDataArray.Num()>WeaponGroupIndex))
	{
		WeaponGroupSelector = WeaponGroupIndex;
		return true;
	}
	else
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : The Index is out of range. Fail to change active weapon group."),*LINE_INFO,*GetNameSafe(GetOwner()));
		return false;
	}
	
}

bool UWeaponControlSystem::SyncNewWeapon(ABaseWeapon * NewWeapon,int WeaponIndex,int WeaponGroupIndex)
{
	if(!IsValid(NewWeapon))
	{
		UE_LOG(Proto,Error,TEXT("%s / %s : Fail to Attach Weapon() / Invalid Weapon"),*LINE_INFO,*GetNameSafe(GetOwner()));
		return false;
	}

	if(!WeaponDataArray[WeaponIndex].SocketArrow_Ref.IsValid())
	{
		UE_LOG(Proto, Error,TEXT("%s / %s : Fail to Attach Weapon(%s) / SocketArrow Reference Error"),*LINE_INFO,*GetNameSafe(GetOwner()),*GetNameSafe(NewWeapon));
		return false;
	}

	NewWeapon->AttachToComponent(WeaponDataArray[WeaponIndex].SocketArrow_Ref.Get(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	if(!NewWeapon->IsAttachedTo(GetOwner()))
	{
		//제대로 Owner에게 붙지 않앗다면 SocketArrow_Ref 등이 잘못된 것이니 다시 떼어낸다.
		NewWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		UE_LOG(Proto, Error,TEXT("%s / %s : Fail to Attach Weapon(%s) / Attached to wrong SocketArrow"),*LINE_INFO,*GetNameSafe(GetOwner()),*GetNameSafe(NewWeapon));

		return false;
	}

	//내부작업

	return true;
}

void UWeaponControlSystem::InitializeWeaponNumber(int NewWeaponNum)
{
	WeaponDataArray.Init(FWeaponData(), NewWeaponNum);
	WeaponDataArray.Reserve(NewWeaponNum);
}

bool UWeaponControlSystem::SetWeaponSocket(class UArrowComponent* NewSocketArrow,int WeaponIndex)
{
	if(WeaponIndex>=WeaponDataArray.Num())
	{
		UE_LOG(Proto, Warning, TEXT("%s / %s : Fail to Set Weapon Socket. WeaponIndex is bigger than WeaponDataArray size"), *LINE_INFO, *GetNameSafe(GetOwner()));
		return false;
	}

	if(IsValid(WeaponDataArray[WeaponIndex].Weapon))
	{
		//만약 해당 인덱스에 이미 무기가 있는경우.(절대 일어나선 안되지만)
		//해당 무기를 파괴시키는 Server 함수를 실행시켜 제거한다.
		//항상 소켓이 무기보다 먼저 세팅되도록 한다.
	}

	WeaponDataArray[WeaponIndex].SocketArrow_Ref = TWeakObjectPtr<UArrowComponent>(NewSocketArrow);

	return WeaponDataArray[WeaponIndex].SocketArrow_Ref.IsValid();
}
