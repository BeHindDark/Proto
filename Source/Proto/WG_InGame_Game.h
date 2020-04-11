// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_InGame_Game.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_InGame_Game : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtScoreRed;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtScoreBlue;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtTime;

public:
	UFUNCTION()
	void SetScore(UTextBlock* txtTMP, int score);
};
