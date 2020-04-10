// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_InGame_Game.h"
#include "Components/TextBlock.h"

void UWG_InGame_Game::NativeConstruct()
{
	Super::NativeConstruct();

	txtScore1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtScore1")));
	if (nullptr == txtScore1) return;

	txtScore2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtScore2")));
	if (nullptr == txtScore2) return;

	txtTime = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtTime")));
	if (nullptr == txtTime) return;	

	//ÃÊ±âÈ­
	txtScore1->SetText(FText::FromString("000"));
	txtScore2->SetText(FText::FromString("000"));
	txtTime->SetText(FText::FromString("00:00"));
}
