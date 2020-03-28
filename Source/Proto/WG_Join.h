// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_Join.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Join : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
		class UButton* JoinButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IDInput;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* PWInput;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* PW2Input;

private:

	UFUNCTION()
	void IDCommitted(const FText& InText, ETextCommit::Type InCommitType);

	UFUNCTION()
	void PassWordCommitted(const FText& InText, ETextCommit::Type InCommitType);
	void PassWord2Committed(const FText& InText, ETextCommit::Type InCommitType);

	UFUNCTION()
	void OnJoinClicked();

	void OnJoinSession();
};
