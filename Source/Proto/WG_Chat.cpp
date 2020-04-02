    // Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Chat.h"
#include "PC_Lobby.h"
#include "WG_TextBox.h"
#include "Components/EditableTextBox.h"

UWG_Chat::UWG_Chat(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWG_TextBox> WG_TextBox_C(TEXT("/Game/Blueprints/Widget/UMG_TextBox.UMG_TextBox_C"));
	if (WG_TextBox_C.Succeeded()) {
		WG_TextBoxClass = WG_TextBox_C.Class;
	}
}

void UWG_Chat::NativeConstruct()
{
	ChatEnty->OnTextCommitted.AddDynamic(this, &UWG_Chat::ChattingCommitted);
}

void UWG_Chat::ChattingCommitted(const FText& Input, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		if (Input.IsEmpty())
		{
			ChatEnty->SetText(Input);
			//GetOwningPlayer()->SetInputMode(GetOwningPlayer())
		}
		else
		{
			APC_Lobby* PC_Lobby = Cast<APC_Lobby>(GetOwningPlayer());
		}
	}
}

void UWG_Chat::AddChatLog(const FText& UserName, const FText& Message)
{
	if (GetOwningPlayer()->IsLocalPlayerController())
	{
		
	}
}
