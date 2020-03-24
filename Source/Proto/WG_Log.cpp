// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Log.h"
#include "Components/Button.h"

void UWG_Log::NativeConstruct()
{
	CheckButton->OnClicked.AddDynamic(this, &UWG_Log::OnCheckButtonClicked);
}

void UWG_Log::OnCheckButtonClicked()
{
	RemoveFromParent();
}
