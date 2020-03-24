// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_Log.generated.h"

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
	class TextBlock* ErrorTypeText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UMultiLineEditableText* ErrorMessageMLEditableText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* CheckButton;

public:

protected:

	UFUNCTION()
	void OnCheckButtonClicked();
};
