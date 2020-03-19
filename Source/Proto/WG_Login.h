// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_Login.generated.h"

/**
 *
 */
UCLASS()
class PROTO_API UWG_Login : public UUserWidget
{
	GENERATED_BODY()


protected:

	virtual void NativeConstruct() override;

protected:

	UPROPERTY(meta = (BindWidget))
	class UButton* LoginButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* CreateAccountButton;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* IDInput;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* PWInput;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* SaveIDCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* UseServerDataCheckBox;

	UPROPERTY(meta = (BindWidget))
	class UCheckBox* UseLocalDataCheckBox;

private:

	UFUNCTION()
	void IDCommitted(const FText& InText, ETextCommit::Type InCommitType);

	UFUNCTION()
	void PassWordCommitted(const FText& InText, ETextCommit::Type InCommitType);

	UFUNCTION()
	void OnLoginClicked();

	void OnLoginSession();

	void OnServerDataCheckBoxClicked(bool Checked);

	void OnLocalDataCheckBoxClicked(bool Checked);
};
