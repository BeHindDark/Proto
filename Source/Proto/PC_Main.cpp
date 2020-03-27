// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Main.h"
#include "WG_Main.h"
#include "WG_SessionBrowser.h"
#include "WG_SessionCreator.h"
#include "WG_Log.h"
#include "Blueprint/UserWidget.h"
#include "GI_Proto.h"

APC_Main::APC_Main()
{
	InitializeWidget();
}

void APC_Main::BeginPlay()
{
	ShowMainWG(1);
	FInputModeUIOnly UIMode;
	SetInputMode(UIMode);

	UGI_Proto* GI_Proto = Cast<UGI_Proto>(GetGameInstance());

	GI_Proto->DestroySessionAndLeaveGame();

}

void APC_Main::ShowMainWG(int Zorder)
{
	if(!IsLocalPlayerController())
	{
		CHECK_LOG(!IsLocalPlayerController());
		return;
	}

	if(IsValid(WG_Main))
	{
		WG_Main->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_Main = nullptr;
		WG_Main = CreateWidget<UWG_Main>(this, WG_Main_Class);
	}
	if(!WG_Main->IsInViewport())
	{
		WG_Main->AddToViewport(Zorder);
	}
	
	
	bShowMouseCursor = true;
}

void APC_Main::ShowSessionBrowserWG(int Zorder)
{
	if(!IsLocalPlayerController())
	{
		CHECK_LOG(!IsLocalPlayerController());
		return;
	}

	if(IsValid(WG_SessionBrowser))
	{
		WG_SessionBrowser->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_SessionBrowser = nullptr;
		WG_SessionBrowser = CreateWidget<UWG_SessionBrowser>(this,WG_SessionBrowser_Class);
	}


	if(!WG_SessionBrowser->IsInViewport())
	{
		WG_SessionBrowser->AddToViewport(Zorder);
	}
	
	
	bShowMouseCursor = true;
}

void APC_Main::ShowSessionCreatorWG(int Zorder)
{
	if(!IsLocalPlayerController())
	{
		CHECK_LOG(!IsLocalPlayerController());
		return;
	}

	if(IsValid(WG_SessionCreator))
	{
		WG_SessionCreator->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_SessionCreator = nullptr;
		WG_SessionCreator = CreateWidget<UWG_SessionCreator>(this,WG_SessionCreator_Class);
	}

	if(!WG_SessionCreator->IsInViewport())
	{
		WG_SessionCreator->AddToViewport(Zorder);
	}

	bShowMouseCursor = true;
}

void APC_Main::ShowLoadingScreenWG(int Zorder)
{
	if(!IsLocalPlayerController())
	{
		CHECK_LOG(!IsLocalPlayerController());
		return;
	}

	if(IsValid(WG_LoadingScreen))
	{
		WG_LoadingScreen->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_LoadingScreen = nullptr;
		WG_LoadingScreen = CreateWidget<UUserWidget>(this,WG_LoadingScreen_Class);
	}

	WG_LoadingScreen->AddToViewport(Zorder);
	
	bShowMouseCursor = false;
}

void APC_Main::ShowLogWG(FString TypeText,FString LogText,int Zorder)
{
	if(!IsLocalPlayerController())
	{
		CHECK_LOG(!IsLocalPlayerController());
		return;
	}

	if(IsValid(WG_Log))
	{
		WG_Log->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_Log = nullptr;
		WG_Log = CreateWidget<UWG_Log>(this,WG_Log_Class);
	}

	WG_Log->InitializeLog(TypeText, LogText, EInputMode::UIOnly);

	WG_Log->AddToViewport(Zorder);

	bShowMouseCursor = true;
}

void APC_Main::OnGetCreateSessionReport(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		
		if(IsValid(WG_SessionCreator))
		{
			WG_SessionCreator->RemoveFromParent();
		}

		if(IsValid(WG_SessionBrowser))
		{
			WG_SessionBrowser->RemoveFromParent();
		}

		if(IsValid(WG_Main))
		{
			WG_Main->RemoveFromParent();
		}


		UGameplayStatics::OpenLevel(GetWorld(),FName("ThirdPersonExampleMap"),true,"listen");
	}
	else
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : Fail to Create Session"),*LINE_INFO,*GetNameSafe(this));

		if(IsValid(WG_LoadingScreen))
		{
			WG_LoadingScreen->RemoveFromParent();
		}
	}
}

void APC_Main::TryJoinOnlineSession(const FOnlineSessionSearchResult& SearchResult)
{
	UGI_Proto* GI_Proto = Cast<UGI_Proto>(GetGameInstance());
	if(!IsValid(GI_Proto))
	{
		CHECK_LOG(!IsValid(GI_Proto));
		return;
	}
	GI_Proto->OnJoinSessionReport.AddUObject(this, &APC_Main::OnGetJoinSessionReport);

	ShowLoadingScreenWG(5);

	GI_Proto->JoinOnlineSession(SearchResult);
}

void APC_Main::OnGetJoinSessionReport(bool bWasSuccessful,FString TravleURL)
{
	if(bWasSuccessful)
	{
		if(IsValid(WG_SessionCreator))
		{
			WG_SessionCreator->RemoveFromParent();
		}

		if(IsValid(WG_SessionBrowser))
		{
			WG_SessionBrowser->RemoveFromParent();
		}

		if(IsValid(WG_Main))
		{
			WG_Main->RemoveFromParent();
		}

		ClientTravel(TravleURL, ETravelType::TRAVEL_Absolute);
	}
	else
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : Fail to Join Session"),*LINE_INFO,*GetNameSafe(this));

		if(IsValid(WG_LoadingScreen))
		{
			WG_LoadingScreen->RemoveFromParent();
		}
	}
}

void APC_Main::InitializeWidget()
{
	/*
	static ConstructorHelpers::FClassFinder<> _C(TEXT(""));
	if(_C.Succeeded())
	{
		_Class = _C.Class;
	}
	*/

	static ConstructorHelpers::FClassFinder<UWG_Main> WG_Main_C(TEXT("/Game/Blueprints/Widget/UMG_Main.UMG_Main_C"));
	if(WG_Main_C.Succeeded())
	{
		WG_Main_Class = WG_Main_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UWG_SessionBrowser> WG_SessionBrowser_C(TEXT("/Game/Blueprints/Widget/UMG_SessionBrowser.UMG_SessionBrowser_C"));
	if(WG_SessionBrowser_C.Succeeded())
	{
		WG_SessionBrowser_Class = WG_SessionBrowser_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UWG_SessionCreator> WG_SessionCreator_C(TEXT("/Game/Blueprints/Widget/UMG_SessionCreator.UMG_SessionCreator_C"));
	if(WG_SessionCreator_C.Succeeded())
	{
		WG_SessionCreator_Class = WG_SessionCreator_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WG_LoadingScreen_C(TEXT("/Game/Blueprints/Widget/UMG_LoadingScreen.UMG_LoadingScreen_C"));
	if(WG_LoadingScreen_C.Succeeded())
	{
		WG_LoadingScreen_Class = WG_LoadingScreen_C.Class;
	}

	static ConstructorHelpers::FClassFinder<UWG_Log> WG_Log_C(TEXT("/Game/Blueprints/Widget/UMG_Log.UMG_Log_C"));
	if(WG_Log_C.Succeeded())
	{
		WG_Log_Class = WG_Log_C.Class;
	}
	
}

