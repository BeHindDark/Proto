// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Main.h"
#include "Components/Button.h"

void UWG_Main::NativeConstruct() {
	Super::NativeConstruct();

	SessionBrowserButton->OnClicked.AddDynamic(this, &UWG_Main::SessionBrowserButtonClicked);
	AmoryButton->OnClicked.AddDynamic(this, &UWG_Main::AmoryButtonClicked);
}

void UWG_Main::SessionBrowserButtonClicked() {

}

void UWG_Main::AmoryButtonClicked() {

}