// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_TeamName.h"
#include "Components/TextBlock.h"

void UWG_TeamName::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerName->SetText(FText::FromString(Name));
}

void UWG_TeamName::SetUserName(FString _UserName)
{
	Name = _UserName;
}
