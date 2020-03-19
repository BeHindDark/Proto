// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionInfo.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/SizeBox.h"

void UWG_SessionInfo::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsEditable(bIsEditable);
	ClearText();
}

void UWG_SessionInfo::SetIsEditable(bool Editable)
{
	bIsEditable = Editable;

	if(bIsEditable)
	{
		SessionNameTextBox->SetIsReadOnly(false);
		DescriptionMLEditableText->SetIsReadOnly(false);
		HostNameTextBox->SetIsReadOnly(true);
		HostNameTextBox->SetText(FText::FromString(FString(TEXT("textsettingtest, hostname"))));
		ScrollSize->SetHeightOverride(500.0f);
	}
	else
	{
		SessionNameTextBox->SetIsReadOnly(true);
		DescriptionMLEditableText->SetIsReadOnly(true);
		HostNameTextBox->SetIsReadOnly(true);
		HostNameTextBox->SetText(FText::FromString(FString(TEXT("textsettingtest, hostname"))));
		ScrollSize->SetHeightOverride(200.0f);
	}
}

void UWG_SessionInfo::ClearText()
{
	if(!bIsEditable)
	{
		HostNameTextBox->SetText(FText::FromString(""));
	}
	SessionNameTextBox->SetText(FText::FromString(""));
	DescriptionMLEditableText->SetText(FText::FromString(""));
}
