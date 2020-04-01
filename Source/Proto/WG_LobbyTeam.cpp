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
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("RedClicked"));
}

void UWG_LobbyTeam::BlueClicked()
{
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("BlueClicked"));
}
