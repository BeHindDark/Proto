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
	
	class UWG_SessionLobby* WG_SessionLobby;

protected:

	TSubclassOf<class UWG_SessionLobby> WG_SessionLobby_Class;
	
public:

	void InitializeWidget();
	void OnAllClicked();
	void OnTeamClicked();
	void OnSquadClicked();

public:

	/*
	 * 최근, 악성 데이터/입력 감지를 위한 관문 역할을 위해 RPC 에 인증(validation) 함수를 추가하는 기능이 생겼습니다.
	 * RPC 에 대한 인증 함수가 악성 파라미터를 감지한 경우,
	 * 해당 RPC 를 호출한 클라이언트/서버 연결을 끊도록 시스템에 알린다는 개념입니다.
	 _Implementation, _Validate 함수 옆에 꼭 붙여줘야된다
	*/

	
	UFUNCTION(Client, Reliable, WithValidation)
	void ClientReceiveChatMessage(const FString& message);
	void ClientReceiveChatMessage_Implementation(const FString& message);
	bool ClientReceiveChatMessage_Validate(const FString& message);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerReceiveChatMessage(const FString& message);
	void ServerReceiveChatMessage_Implementation(const FString& message);
	bool ServerReceiveChatMessage_Validate(const FString& message);


	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void MulticastTeam(const FString& userName);
	void MulticastTeam_Implementation(const FString& userName);
	bool MulticastTeam_Validate(const FString& userName);
};