// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionBrowser.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
#include "WG_SessionList.h"
#include "WG_SessionLine.h"
#include "Components/ScrollBox.h"

UWG_SessionBrowser::UWG_SessionBrowser(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UWG_SessionLine> WG_SessionLine_C(TEXT("/Game/Blueprints/Widget/UMG_SessionLine.UMG_SessionLine_C"));
	if(WG_SessionLine_C.Succeeded()) {
		WG_SessionLine_Class = WG_SessionLine_C.Class;
	}
}


void UWG_SessionBrowser::NativeConstruct()
{
	Super::NativeConstruct();
	
	for(int i=0; i<5; i++)
	{
		UWG_SessionLine* SessionNewLine = CreateWidget<UWG_SessionLine>(GetOwningPlayer(), WG_SessionLine_Class);
		WG_SessionList->SessionScrollBox->AddChild(SessionNewLine);
		SessionNewLine->SetUpperClass(WG_SessionList, this);
	}
	
	
}