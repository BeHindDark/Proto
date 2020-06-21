// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Pilot.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DamageControlSystem.h"

void UWG_Pilot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWG_Pilot::ConnectDamageControlSystem(UDamageControlSystem* NewSystem)
{
	if(IsValid(GetOwningPlayer()))
	{
		if(!GetOwningPlayer()->IsLocalPlayerController())
		{
			return;
		}
	}
	else
	{
		return;
	}

	DamageControlSystem_Ref = TWeakObjectPtr<UDamageControlSystem>(NewSystem);


	NewSystem->OnHPIsChanged.AddUObject(this, &UWG_Pilot::OnHPIsChanged);
		
}

void UWG_Pilot::SetClassName(const FName& ConnectedClassName)
{
	/*
		이부분은 수정되어야 한다.어떤 클래스인지 몰라도 각종 정보를 받거나,델리게이트를 이용할 수 있게
		외부와의 일반적인 소통을 담당하는 액터클래스를 하나더 만들어야 할 것 같다?
		WeaponControlSystem : 무기조종 및 연결
		DamageControlSystem : 데미지 처리
	*/
	ClassNameText->SetText(FText::FromName(ConnectedClassName));
}

void UWG_Pilot::OnHPIsChanged()
{
	if(!(DamageControlSystem_Ref.IsValid()||IsValid(HPProgressBar))) return;

	HPProgressBar->SetPercent(DamageControlSystem_Ref.Get()->GetHPRatio());
}