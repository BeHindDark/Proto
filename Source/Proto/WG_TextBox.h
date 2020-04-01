// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_TextBox.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_TextBox : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* UserNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ChatText;
	
};
