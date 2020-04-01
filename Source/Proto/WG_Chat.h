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
	
protected:
	virtual void NativeConstruct() override;

protected:
	
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ChatEnty;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ChatLog;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatMode;

private:

	UFUNCTION()
	void ChattingCommitted(const FText& Input, ETextCommit::Type InCommitType);

};
