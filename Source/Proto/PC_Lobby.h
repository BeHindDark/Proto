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
	 * �ֱ�, �Ǽ� ������/�Է� ������ ���� ���� ������ ���� RPC �� ����(validation) �Լ��� �߰��ϴ� ����� ������ϴ�.
	 * RPC �� ���� ���� �Լ��� �Ǽ� �Ķ���͸� ������ ���,
	 * �ش� RPC �� ȣ���� Ŭ���̾�Ʈ/���� ������ ������ �ý��ۿ� �˸��ٴ� �����Դϴ�.
	 _Implementation, _Validate �Լ� ���� �� �ٿ���ߵȴ�
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