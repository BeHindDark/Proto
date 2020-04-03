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
	InputComponent->BindAction(TEXT("AllChat"), EInputEvent::IE_Pressed, this, &APC_Lobby::OnAllClicked);
	InputComponent->BindAction(TEXT("TeamChat"), EInputEvent::IE_Pressed, this, &APC_Lobby::OnTeamClicked);
	InputComponent->BindAction(TEXT("SquadChat"), EInputEvent::IE_Pressed, this, &APC_Lobby::OnSquadClicked);

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

	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("OnAllClicked"));
}

void APC_Lobby::OnTeamClicked()
{
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("OnTeamClicked"));
}

void APC_Lobby::OnSquadClicked()
{
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("OnSquadClicked"));
}



void APC_Lobby::ClientReceiveChatMessage_Implementation(const FString& message)
{
}

bool APC_Lobby::ClientReceiveChatMessage_Validate(const FString& message)
{
	return false;
}

void APC_Lobby::ServerSendChatMessage_Implementation(const FString& message)
{
	for (auto Iter = GetWorld()->GetPlayerControllerIterator(); Iter; ++Iter)
	{
		auto PC = Cast<APC_Lobby>(*Iter);
		if (PC)
		{

		}
	}
}

bool APC_Lobby::ServerSendChatMessage_Validate(const FString& message)
{
	if (message.Len() < 255) return true;
	else return false;
}

void APC_Lobby::ClientSendChatMessage_Implementation(const FString& message)
{
	if (nullptr == WG_SessionLobby)
	{
		return;
	}

}

bool APC_Lobby::ClientSendChatMessage_Validate(const FString& message)
{
	return true;
}