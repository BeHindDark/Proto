// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/PlayerController.h"
#include "PC_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API APC_Lobby : public APlayerController
{
	GENERATED_BODY()

		APC_Lobby();

protected:

	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

public:
	class UWG_Chat* WG_Chat;

	class UWG_LobbyTeam* WG_LobbyTeam;

	class UWG_SessionLobby* WG_SessionLobby;

	class UWG_ChatBox* WG_textBox;

protected:

	TSubclassOf<class UWG_Chat> WG_Chat_Class;

	TSubclassOf<class UWG_LobbyTeam> WG_LobbyTeam_Class;

	TSubclassOf<class UWG_SessionLobby> WG_SessionLobby_Class;

public:

	void InitializeWidget();
	void OnAllClicked();
	void OnTeamClicked();
	void OnSquadClicked();
};