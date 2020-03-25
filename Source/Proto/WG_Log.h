// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_Log.generated.h"

UENUM(BlueprintType)
enum class EUMGLogType:uint8
{
	Notification	UMETA(DisplayName = "Notification"),
	Warning			UMETA(DisplayName = "Warning"),
	Error			UMETA(DisplayName = "Error")
};

UENUM(BlueprintType)
enum class EInputMode:uint8
{
	UIOnly		UMETA(DisplayName = "UIOnly"),
	GameOnly	UMETA(DisplayName = "GameOnly"),
	GameAndUI	UMETA(DisplayName = "GameAndUI")
};

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Log : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UTextBlock* LogTypeText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UMultiLineEditableText* LogMessageMLEditableText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* CheckButton;

	EInputMode PreviousInputMode;

public:
	void InitializeLog(const EUMGLogType& UMGLogType, FString LogText, const EInputMode& InputMode);

	void InitializeLog(FString UMGLogType, FString LogText, const EInputMode& InputMode);

protected:

	UFUNCTION()
	void OnCheckButtonClicked();
};
