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

	//초기화
	txtScoreRed->SetText(FText::FromString("000"));
	txtScoreBlue->SetText(FText::FromString("000"));
	txtTime->SetText(FText::FromString("00:00"));
		
}

void UWG_InGame_Game::SetScore(ScoreType team, int score)
{
	FText textScore;

	// int형 hp를 FText형으로 변환
	if (score >= 100) {
		textScore = (FText::FromString(FString::FromInt(score)));
	}
	else if (score <= 0) {
		textScore = (FText::FromString("000"));
	}
	else {
		textScore = (FText::FromString("0" + FString::FromInt(score)));
	}

	switch (team) {
	case ScoreType::Red:
		txtScoreRed->SetText(textScore);
		break;
	case ScoreType::Blue:
		txtScoreBlue->SetText(textScore);
		break;	
	}
}
