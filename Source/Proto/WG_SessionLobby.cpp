// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionLobby.h"
#include "PC_Lobby.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"


void UWG_SessionLobby::NativeConstruct()
{
	Super::NativeConstruct();
	
	ChatInput->OnTextCommitted.AddDynamic(this, &UWG_SessionLobby::OnInputChatCommit);

	RedButton->OnClicked.AddDynamic(this, &UWG_SessionLobby::RedClicked);
	BlueButton->OnClicked.AddDynamic(this, &UWG_SessionLobby::BlueClicked);
	
	GameStartButton->OnClicked.AddDynamic(this, &UWG_SessionLobby::OnGameStartClicked);

	/*
	APlayerState* t = Cast<APlayerState>(GetOwningPlayer());
	t->Tags.Add(FName("mytag"));
	t->Tags.Find(FName("mytag"));
	*/
}

void UWG_SessionLobby::OnGameStartClicked()
{
	GEngine->AddOnScreenDebugMessage(10, 10, FColor::Red, TEXT("GameStartButton Clicked"));
	
}



void UWG_SessionLobby::RedClicked()
{
	UE_LOG(Proto, Error, TEXT("Red"));
	
}

void UWG_SessionLobby::BlueClicked()
{
	UE_LOG(Proto, Error, TEXT("Blue"));
}

void UWG_SessionLobby::UpdateRedList(const FString& Name)
{
	FString RedName = FString::Printf(TEXT("%s"), *Name);

	
	
}

void UWG_SessionLobby::UpdateBlueList(const FString& Name)
{
	
	RedPlayerList->ClearChildren();
	BluePlayerList->ClearChildren();
}



void UWG_SessionLobby::UpdateChatBox(const FString& message)
{
	FString Chat = FString::Printf(TEXT("%s"), *message);

	UTextBlock* NewText = NewObject<UTextBlock>(ChatBox);
	NewText->SetText(FText::FromString(Chat));
	NewText->SetAutoWrapText(true);

	ChatBox->AddChild(NewText);
	ChatBox->ScrollToEnd();
}


void UWG_SessionLobby::OnInputChatCommit(const FText& Input, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		auto MyPC = Cast<APC_Lobby>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		if (nullptr == MyPC)
		{
			return;
		}

		MyPC->ServerReceiveChatMessage(Input.ToString());
		if (ChatInput)
		{
			ChatInput->SetText(FText::GetEmpty());
		}
	}
	
}
