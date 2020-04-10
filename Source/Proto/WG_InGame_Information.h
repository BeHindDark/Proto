// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_InGame_Information.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_InGame_Information : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtBodyHP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponeHP1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponeHP2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponeHP3;
};
