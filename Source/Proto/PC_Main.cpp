// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Main.h"
#include "WG_Main.h"

APC_Main::APC_Main()
{
	static ConstructorHelpers::FClassFinder<UWG_Main> WG_MAIN_C(TEXT("/Game/Blueprints/Widget/WG_Main.WG_Main_C"));
	if (WG_MAIN_C.Succeeded())
	{
		WG_Main_Class = WG_MAIN_C.Class;
	}
}

void APC_Main::BeginPlay()
{
	WG_Main = CreateWidget<UWG_Main>(this, WG_Main_Class);
	WG_Main->AddToViewport();
	bShowMouseCursor = true;
}
