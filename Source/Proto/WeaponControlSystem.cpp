// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponControlSystem.h"
#include "Act_WeaponBase.h"
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


void UWeaponControlSystem::ActivateWeaponGroup_Implementation(int WeaponGroupIndex)
{
	if((WeaponGroupIndex>=0)&&(WeaponGroupArray.Num()>WeaponGroupIndex))
	{
		if(WeaponGroupArray[WeaponGroupIndex].Num()>0)
		{
			WeaponGroupSelector = WeaponGroupIndex;
			return;
		}
		else
		{
			UE_LOG(Proto,Warning,TEXT("%s / %s : There is no registered Weapon in WeaponGroup(%d)."),*LINE_INFO,*GetNameSafe(GetOwner()),WeaponGroupIndex);
			return;
		}

	}
	else
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : The Index is out of range. Fail to change active weapon group."),*LINE_INFO,*GetNameSafe(GetOwner()));
		return;
	}
}

bool UWeaponControlSystem::ActivateWeaponGroup_Validate(int WeaponGroupIndex)
{
	return true;
}


bool UWeaponControlSystem::SyncNewWeapon(AAct_WeaponBase * NewWeapon,int WeaponIndex,int WeaponGroupIndex)
{
	//오류 처리
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

	//Attach 시도
	NewWeapon->AttachToComponent(WeaponDataArray[WeaponIndex].SocketArrow_Ref.Get(), FAttachmentTransformRules::SnapToTargetIncludingScale);

	//Attach 실패 처리
	if(!NewWeapon->IsAttachedTo(GetOwner()))
	{
		//제대로 Owner에게 붙지 않앗다면 SocketArrow_Ref 등이 잘못된 것이니 다시 떼어낸다.
		NewWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		UE_LOG(Proto, Error,TEXT("%s / %s : Fail to Attach Weapon(%s) / Attached to wrong SocketArrow"),*LINE_INFO,*GetNameSafe(GetOwner()),*GetNameSafe(NewWeapon));

		return false;
	}

	//내부작업

	WeaponDataArray[WeaponIndex].Weapon = NewWeapon;
	WeaponDataArray[WeaponIndex].GroupIndex = WeaponGroupIndex;
	NewWeapon->ConnectWeaponControlSystem(this, WeaponIndex);

	//기존 WeaponIndex 제거
	for(TArray<int> WeaponIndexArray :WeaponGroupArray)
	{
		WeaponIndexArray.Remove(WeaponIndex);
	}

	//다시 WeaponIndex 추가
	WeaponGroupArray[WeaponGroupIndex].AddUnique(WeaponIndex);

	return true;
}

void UWeaponControlSystem::InitializeWeaponNumber(int32 NewWeaponNum)
{
	//WeaponDataArray와 WeaponGroupArray를 초기화 시키고, 해당 TArray에 메모리를 할당한다.
	//Reserve로 미리 메모리 공간을 확보하여, Add 등으로 TArray의 크기가 변할때 공간이 부족하여 새로운 메모리를 할당하는 일이 없도록 방지한다.(최적화)
	if(NewWeaponNum <=0)
	{
		NewWeaponNum = 1;
	}
	WeaponDataArray.Init(FWeaponData(), NewWeaponNum);
	WeaponDataArray.Reserve(NewWeaponNum);
	WeaponGroupArray.Init(TArray<int>(), NewWeaponNum);
	WeaponGroupArray.Reserve(NewWeaponNum);
}

bool UWeaponControlSystem::SetWeaponSocket(class UArrowComponent* NewSocketArrow,int WeaponIndex, const TArray<float>& YawLimit,const TArray<float>& PitchLimit)
{
	//주어진 WeaponIndex가 가질 수 있는 무기 수보다 클 떄
	if(WeaponIndex>=WeaponDataArray.Num())
	{
		CHECK_LOG(WeaponIndex>=WeaponDataArray.Num());
		UE_LOG(Proto, Error, TEXT("%s / %s : Fail to Set Weapon Socket. WeaponIndex is bigger than WeaponDataArray size"), *LINE_INFO, *GetNameSafe(GetOwner()));
		return false;
	}

	//Yaw와 Pitch 회전 제한 TArray의 사이즈가 2가 아닐 시 에러
	if((YawLimit.Num()!=2)||(PitchLimit.Num()!=2))
	{
		CHECK_LOG((YawLimit.Num()!=2)||(PitchLimit.Num()!=2));
		UE_LOG(Proto,Error,TEXT("%s / %s : Fail to Set Weapon Socket. Pitch and Yaw Limit Size is invalid"),*LINE_INFO,*GetNameSafe(GetOwner()));
		return false;
	}

	//Yaw와 Pitch 회전 제한 TArray의 [0]가 [1]보다 작지 않을 시 
	if(!((YawLimit[0]<YawLimit[1])&&(PitchLimit[0]<PitchLimit[1])))
	{
		CHECK_LOG(!((YawLimit[0]<YawLimit[1])&&(PitchLimit[0]<PitchLimit[1])));
		UE_LOG(Proto,Error,TEXT("%s / %s : Fail to Set Weapon Socket. Pitch and Yaw Limit Value is invalid"),*LINE_INFO,*GetNameSafe(GetOwner()));
		return false;
	}

	//만약 해당 인덱스에 이미 무기가 있는경우.(절대 일어나선 안되지만)
	if(IsValid(WeaponDataArray[WeaponIndex].Weapon))
	{
		//해당 무기를 파괴시키는 Server 함수를 실행시켜 제거한다.
		//항상 소켓이 무기보다 먼저 세팅되도록 한다.
	}

	WeaponDataArray[WeaponIndex].SocketArrow_Ref = TWeakObjectPtr<UArrowComponent>(NewSocketArrow);
	WeaponDataArray[WeaponIndex].YawRotationLimit = YawLimit;
	WeaponDataArray[WeaponIndex].PitchRotationLimit = PitchLimit;

	return WeaponDataArray[WeaponIndex].SocketArrow_Ref.IsValid();
}

void UWeaponControlSystem::SendFireOrder()
{
	for(int WeaponIndex:WeaponGroupArray[WeaponGroupSelector])
	{
		WeaponDataArray[WeaponIndex].Weapon->ServerOnFireOrder();
	}
}

void UWeaponControlSystem::SendCeaseFireOrder()
{
	for(int WeaponIndex:WeaponGroupArray[WeaponGroupSelector])
	{
		WeaponDataArray[WeaponIndex].Weapon->ServerOnCeaseFireOrder();
	}
}
