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


void APC_Lobby::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APC_Lobby::AttemptToSendChatMessage(const FString& Message)
{
	//if (Role < ROLE_Authority) ServerSendChatMessage(Message, ChatMode);
}

void APC_Lobby::ServerSendChatMessage_Implementation(const FString& message)
{

}

bool APC_Lobby::ServerSendChatMessage_Validate(const FString& message)
{
	if (message.Len() < 255) return true;
	else return false;
}

void APC_Lobby::ClientSendChatMessage_Implementation(const FString& message)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("����Ȯ��"));
}

void APC_Lobby::SendChatMessage(const FString& Message)
{
	
}