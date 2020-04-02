// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_Chat.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Chat : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UWG_Chat(const FObjectInitializer& ObjectInitializer);

protected:

	virtual void NativeConstruct() override;

	TSubclassOf<class UWG_TextBox> WG_TextBoxClass;

public:
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatEnty;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatLog;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMode;

	class UWG_TextBox* WG_TextBox;

private:

	UFUNCTION()
	void ChattingCommitted(const FText& Input, ETextCommit::Type InCommitType);

	UFUNCTION()
	void AddChatLog(const FText& UserName, const FText& Message);

};
