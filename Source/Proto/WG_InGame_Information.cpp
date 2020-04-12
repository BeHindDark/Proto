// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_InGame_Information.h"
#include "Components/TextBlock.h"
#include "Components/SizeBox.h"

void UWG_InGame_Information::NativeConstruct()
{
	Super::NativeConstruct();

	txtBodyHP = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtBodyHP")));
	if (nullptr == txtBodyHP) return;

	txtWeaponHP1 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponHP1")));
	if (nullptr == txtWeaponHP1) return;

	txtWeaponHP2 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponHP2")));
	if (nullptr == txtWeaponHP2) return;

	txtWeaponHP3 = Cast<UTextBlock>(GetWidgetFromName(TEXT("txtWeaponHP3")));
	if (nullptr == txtWeaponHP3) return;

	boxWeaponHP1 = Cast<USizeBox>(GetWidgetFromName(TEXT("boxWeaponHP1")));
	if (nullptr == boxWeaponHP1) return;

	boxWeaponHP2 = Cast<USizeBox>(GetWidgetFromName(TEXT("boxWeaponHP2")));
	if (nullptr == boxWeaponHP2) return;

	boxWeaponHP3 = Cast<USizeBox>(GetWidgetFromName(TEXT("boxWeaponHP3")));
	if (nullptr == boxWeaponHP3) return;

	//ÃÊ±âÈ­
	txtBodyHP->SetText(FText::FromString("100"));	
	txtWeaponHP1->SetText(FText::FromString("100"));
	txtWeaponHP2->SetText(FText::FromString("100"));
	txtWeaponHP3->SetText(FText::FromString("100"));

	UseWeapon1 = false;
	UseWeapon2 = false;
	UseWeapon3 = false;
}

void UWG_InGame_Information::SetHP(int hp)
{
	if (hp >= 100) {
		txtBodyHP->SetText(FText::FromString("100"));
	}
	else if (hp <= 0) {
		txtBodyHP->SetText(FText::FromString("000"));
	}
	else {
		txtBodyHP->SetText(FText::FromString("0" + FString::FromInt(hp)));
	}
}

void UWG_InGame_Information::SetUseWeapon(bool used1, bool used2, bool used3)
{
	UseWeapon1 = used1;
	UseWeapon2 = used2;
	UseWeapon3 = used3;

	if (UseWeapon1) {
		boxWeaponHP1->SetVisibility(ESlateVisibility::Hidden);
	}

	if (UseWeapon2) {
		boxWeaponHP3->SetVisibility(ESlateVisibility::Hidden);
	}

	if (UseWeapon3) {
		boxWeaponHP3->SetVisibility(ESlateVisibility::Hidden);
	}
}