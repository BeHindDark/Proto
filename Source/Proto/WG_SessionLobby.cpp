// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionLobby.h"
#include "WG_Chat.h"
#include "WG_LobbyTeam.h"
#include "Components/Button.h"

void UWG_SessionLobby::NativeConstruct()
{
	Super::NativeConstruct();

	GameStartButton->OnClicked.AddDynamic(this, &UWG_SessionLobby::OnGameStartClicked);
}

void UWG_SessionLobby::OnGameStartClicked()
{
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Red, TEXT("GameStartButton Clicked"));
}
