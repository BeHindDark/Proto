// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Main.h"
#include "Components/Button.h"
#include "PC_Main.h"

void UWG_Main::NativeConstruct()
{
	Super::NativeConstruct();

	SessionBrowserButton->OnClicked.AddDynamic(this, &UWG_Main::SessionBrowserButtonClicked);
	AmoryButton->OnClicked.AddDynamic(this, &UWG_Main::AmoryButtonClicked);
}

void UWG_Main::SessionBrowserButtonClicked()
{
	
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());

	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main))
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		PC_Main->ShowSessionBrowserWG(2);
	}
}

void UWG_Main::AmoryButtonClicked()
{

}