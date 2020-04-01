// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Join.h"
#include "PC_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UWG_Join::NativeConstruct()
{
	Super::NativeConstruct();
	
	IDInput->OnTextCommitted.AddDynamic(this, &UWG_Join::IDCommitted);
	PWInput->OnTextCommitted.AddDynamic(this, &UWG_Join::PassWordCommitted);
	PW2Input->OnTextCommitted.AddDynamic(this, &UWG_Join::PassWordCommitted);
	JoinButton->OnClicked.AddDynamic(this, &UWG_Join::OnJoinClicked);
	
}

void UWG_Join::IDCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		PWInput->SetKeyboardFocus();
	}
}

void UWG_Join::PassWordCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		PW2Input->SetKeyboardFocus();
	}
}

void UWG_Join::PassWord2Committed(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		UWG_Join::OnJoinSession();
	}
}

void UWG_Join::OnJoinClicked()
{
	UWG_Join::OnJoinSession();
}

void UWG_Join::OnJoinSession()
{

	if (PWInput->GetText().ToString().Len() != 0 && PW2Input->GetText().ToString().Len() != 0) 
	{
		if (PWInput->GetText().ToString() == PW2Input->GetText().ToString())
		{
			if (IDInput->GetText().ToString().Len() >= 5) 
			{
				
			}
			else 
			{
				GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Recommend to ID Length is over five"));
			}
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
	}
	
}
