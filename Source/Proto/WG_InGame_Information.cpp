// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_InGame_Information.h"
#include "Components/TextBlock.h"

void UWG_InGame_Information::NativeConstruct()
{
	Super::NativeConstruct();

	txtBodyHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtBodyHP")));
	if (nullptr == txtBodyHP) return;

	txtWeaponeHP1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponeHP1")));
	if (nullptr == txtWeaponeHP1) return;

	txtWeaponeHP2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponeHP2")));
	if (nullptr == txtWeaponeHP2) return;

	txtWeaponeHP3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponeHP3")));
	if (nullptr == txtWeaponeHP3) return;

	//ÃÊ±âÈ­
	txtBodyHP->SetText(FText::FromString("100"));	
	txtWeaponeHP1->SetText(FText::FromString("100"));
	txtWeaponeHP2->SetText(FText::FromString("100"));
	txtWeaponeHP3->SetText(FText::FromString("100"));
}
