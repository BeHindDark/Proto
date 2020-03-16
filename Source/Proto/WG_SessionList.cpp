// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionList.h"
#include "WG_SessionLine.h"
#include "Components/ScrollBox.h"

void UWG_SessionList::NativeConstruct()
{
	Super::NativeConstruct();
	/*
	for(UWidget* ChildLine : SessionScrollBox->GetAllChildren())
	{

	}
	*/
}

void UWG_SessionList::ClearSelection()
{
	for(UWidget* Child : SessionScrollBox->GetAllChildren())
	{
		UWG_SessionLine* ChildLine = Cast<UWG_SessionLine>(Child);
		if(ChildLine==nullptr)
		{
			//오류메세지
			CHECK_LOG(ChildLine==nullptr);
		}
		else
		{
			if(ChildLine->GetIsSelected())
			{
				ChildLine->SetSelect(false);
			}
		}
	}
}
