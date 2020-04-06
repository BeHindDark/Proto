// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Main.h"
#include "Components/Button.h"
#include "PC_Main.h"
#include "LoginHttp.h"
#include "GI_Proto.h"
#include "Components/TextBlock.h"

void UWG_Main::NativeConstruct()
{
	Super::NativeConstruct();

	UserIDText = Cast<UTextBlock>(GetWidgetFromName(TEXT("UserIDText")));
	if (nullptr == UserIDText) return;

	//MyGameInstance->GetWebConnector().GetUserNickname()
	
	SessionBrowserButton->OnClicked.AddDynamic(this, &UWG_Main::SessionBrowserButtonClicked);
	AmoryButton->OnClicked.AddDynamic(this, &UWG_Main::AmoryButtonClicked);

	auto MyGameInstance = GetGameInstance<UGI_Proto>();
	if (MyGameInstance)
	{
		UserIDText->SetText(FText::FromString(MyGameInstance->GetWebconnect().GetUserId()));
	}

}

void UWG_Main::SessionBrowserButtonClicked()
{
	
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());

	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main))
	}
	else
	{
		// 자기 자신을 숨기는 함수
		SetVisibility(ESlateVisibility::Hidden);
		PC_Main->ShowSessionBrowserWG(2);
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
