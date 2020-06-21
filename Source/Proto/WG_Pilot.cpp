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
		�̺κ��� �����Ǿ�� �Ѵ�.� Ŭ�������� ���� ���� ������ �ްų�,��������Ʈ�� �̿��� �� �ְ�
		�ܺο��� �Ϲ����� ������ ����ϴ� ����Ŭ������ �ϳ��� ������ �� �� ����?
		WeaponControlSystem : �������� �� ����
		DamageControlSystem : ������ ó��
	*/
	ClassNameText->SetText(FText::FromName(ConnectedClassName));
}

void UWG_Pilot::OnHPIsChanged()
{
	if(!(DamageControlSystem_Ref.IsValid()||IsValid(HPProgressBar))) return;

	HPProgressBar->SetPercent(DamageControlSystem_Ref.Get()->GetHPRatio());
}