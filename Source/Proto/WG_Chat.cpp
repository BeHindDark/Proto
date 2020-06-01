    // Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Chat.h"
#include "PC_Lobby.h"
#include "WG_ChatLog.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"

UWG_Chat::UWG_Chat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	
}

void UWG_Chat::NativeConstruct()
{
	ChatEntry->OnTextCommitted.AddDynamic(this, &UWG_Chat::OnInputChatCommit);
}

void UWG_Chat::UpdateChatBox(const FString& message)
{
	FString Chat = FString::Printf(TEXT("%s"), *message);

	UTextBlock* NewText = NewObject<UTextBlock>(ChatLog);
	NewText->SetText(FText::FromString(Chat));
	NewText->SetAutoWrapText(true);

	ChatLog->AddChild(NewText);
	ChatLog->ScrollToEnd();
}

void UWG_Chat::OnInputChatCommit(const FText& Input, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		if (Input.IsEmpty())
		{
			ChatEntry->SetText(Input);

		}
		else
		{
			APC_Lobby* PC_Lobby = Cast<APC_Lobby>(GetOwningPlayer());
		}
	}
}

