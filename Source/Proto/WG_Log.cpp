// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Log.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/MultiLineEditableText.h"

void UWG_Log::NativeConstruct()
{
	CheckButton->OnClicked.AddDynamic(this, &UWG_Log::OnCheckButtonClicked);
}

void UWG_Log::InitializeLog(const EUMGLogType& UMGLogType, FString LogText, const EInputMode& InputMode)
{
	if((!IsValid(LogTypeText))||(!IsValid(LogMessageMLEditableText))||(!IsValid(CheckButton)))
	{
		RemoveFromParent();
		return;
	}

	switch(UMGLogType)
	{
		case EUMGLogType::Notification :
			LogTypeText->SetText(FText::FromString(FString(TEXT("Notification"))));
			break;
		case EUMGLogType::Warning:
			LogTypeText->SetText(FText::FromString(FString(TEXT("Warning"))));
			break;
		case EUMGLogType::Error :
			LogTypeText->SetText(FText::FromString(FString(TEXT("Error"))));
			break;
		default:
			LogTypeText->SetText(FText::FromString(FString(TEXT("None"))));
	}

	PreviousInputMode = InputMode;

	LogMessageMLEditableText->SetText(FText::FromString(LogText));
}

void UWG_Log::InitializeLog(FString UMGLogType,FString LogText,const EInputMode & InputMode)
{
	LogTypeText->SetText(FText::FromString(UMGLogType));
	LogMessageMLEditableText->SetText(FText::FromString(LogText));
	PreviousInputMode = InputMode;
}

void UWG_Log::OnCheckButtonClicked()
{
	switch(PreviousInputMode)
	{
		case EInputMode::UIOnly :
			GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
			break;
		case EInputMode::GameOnly:
			GetOwningPlayer()->SetInputMode(FInputModeGameOnly());
			break;
		case EInputMode::GameAndUI:
			GetOwningPlayer()->SetInputMode(FInputModeGameAndUI());
			break;
		default:
			GetOwningPlayer()->SetInputMode(FInputModeUIOnly());
			break;
	}
	RemoveFromParent();
}
