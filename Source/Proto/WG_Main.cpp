// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Main.h"
#include "Components/Button.h"
#include "PC_Main.h"
#include "LoginHttp.h"
#include "Components/TextBlock.h"

void UWG_Main::NativeConstruct()
{
	Super::NativeConstruct();

	UserIDText = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserIDText")));
	if (nullptr == UserIDText) return;

	UserIDText->SetText(FText::FromString("test1"));
	SessionBrowserButton->OnClicked.AddDynamic(this, &UWG_Main::SessionBrowserButtonClicked);
	AmoryButton->OnClicked.AddDynamic(this, &UWG_Main::AmoryButtonClicked);
}

void UWG_Main::SessionBrowserButtonClicked()
{
	UE_LOG(Proto, Warning, TEXT("button clicked"));
	//약포인터를 써야하나?
	//TWeakPtr<APC_Main> PC_Main = MakeShared<APC_Main>(Cast<APC_Main>(GetOwningPlayer()));
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());

	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main))
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		PC_Main->ShowSessionBrowserWG();
	}
}

void UWG_Main::AmoryButtonClicked()
{

}

void UWG_Main::UserUpdate()
{
	//FActorSpawnParameters spawnparams;
	//spawnparams.Owner = this;
	//ALoginHttp* Login_Http = GetWorld()->SpawnActor<ALoginHttp>(FVector::ZeroVector, FRotator::ZeroRotator, spawnparams);
}
