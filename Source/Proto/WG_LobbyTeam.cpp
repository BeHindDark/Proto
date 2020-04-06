// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_LobbyTeam.h"
#include "Components/Button.h"

void UWG_LobbyTeam::NativeConstruct()
{
	Super::NativeConstruct();
	RedButton->OnClicked.AddDynamic(this, &UWG_LobbyTeam::RedClicked);
	BlueButton->OnClicked.AddDynamic(this, &UWG_LobbyTeam::BlueClicked);
}

void UWG_LobbyTeam::RedClicked()
{
	UE_LOG(Proto, Error, TEXT("Red"));
}

void UWG_LobbyTeam::BlueClicked()
{
	UE_LOG(Proto, Error, TEXT("Blue"));
}
