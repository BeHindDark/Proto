// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Lobby.h"
#include "WG_SessionLobby.h"

APC_Lobby::APC_Lobby()
{
	InitializeWidget();
}

void APC_Lobby::BeginPlay()
{

}

void APC_Lobby::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void APC_Lobby::InitializeWidget()
{
	static ConstructorHelpers::FClassFinder<UWG_SessionLobby> WG_SessionLobby_C(TEXT("/Game/Blueprints/Widget/UMG_SessionLobyy.UMG_SessionLobyy_C"));
	if (WG_SessionLobby_C.Succeeded()) {
		WG_SessionLobby_Class = WG_SessionLobby_C.Class;
	}
}

void APC_Lobby::OnAllClicked()
{

}

void APC_Lobby::OnTeamClicked()
{

}

void APC_Lobby::OnSquadClicked()
{

}
