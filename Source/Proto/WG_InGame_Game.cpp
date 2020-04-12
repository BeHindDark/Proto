// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_InGame_Game.h"
#include "Components/TextBlock.h"

void UWG_InGame_Game::NativeConstruct()
{
	Super::NativeConstruct();

	txtScoreRed = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtScoreRed")));
	if (nullptr == txtScoreRed) return;

	txtScoreBlue = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtScoreBlue")));
	if (nullptr == txtScoreBlue) return;

	txtTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTime")));
	if (nullptr == txtTime) return;	

	//ÃÊ±âÈ­
	txtScoreRed->SetText(FText::FromString("000"));
	txtScoreBlue->SetText(FText::FromString("000"));
	txtTime->SetText(FText::FromString("00:00"));
		
}

void UWG_InGame_Game::SetScore(UTextBlock* txtTMP, int score)
{
	if (score >= 100) {
		txtTMP->SetText(FText::FromString(FString::FromInt(score)));
	}
	else if (score <= 0) {
		txtTMP->SetText(FText::FromString("000"));
	}
	else {
		txtTMP->SetText(FText::FromString("0" + FString::FromInt(score)));
	}
}
