﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionBrowser.h"
#include "WG_SessionList.h"
#include "WG_SessionLine.h"
#include "WG_SessionInfo.h"
#include "GI_Proto.h"
#include "PC_Main.h"
#include "Components/CanvasPanel.h"
#include "Components/Button.h"
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
	
	JoinButton->OnClicked.AddDynamic(this, &UWG_SessionBrowser::OnJoinButtonClicked);
	RefreshAllButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnRefreshAllButtonClicked);	
	SessionCreatorButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnSessionCreatorButtonClicked);
	BacktoMainButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnBacktoMainButtonClicked);
	QuickRefreshButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnQuickRefreshButtonClicked);

	//CallFindSession();
}

void UWG_SessionBrowser::OnGetFindSessionReport(bool bWasSuccessful)
{
	SetVisibility(ESlateVisibility::Visible);

	UGI_Proto* GI_Proto = Cast<UGI_Proto>(GetGameInstance());
	if(!IsValid(GI_Proto))
	{
		CHECK_LOG(!IsValid(GI_Proto));
		return;
	}
	GI_Proto->OnFindSessionReport.Clear();

	if(bWasSuccessful)
	{
		WG_SessionList->SessionScrollBox->ClearChildren();

		for(FOnlineSessionSearchResult SearchResult : GI_Proto->SessionSearch->SearchResults)
		{
			if(SearchResult.IsValid())
			{
				UWG_SessionLine* SessionNewLine = CreateWidget<UWG_SessionLine>(GetOwningPlayer(),WG_SessionLine_Class);
				WG_SessionList->SessionScrollBox->AddChild(SessionNewLine);
				SessionNewLine->ConnectBrowser(this);
				SessionNewLine->UpdateSessionData(SearchResult);
			}
		}
	}
	else
	{
		UE_LOG(Proto, Warning, TEXT("%s / %s : Fail to Find Sessions"), *LINE_INFO, *GetNameSafe(this));
	}
}

void UWG_SessionBrowser::OnJoinButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

	PC_Main->TryJoinOnlineSession(WG_SessionInfo->SessionData);

}

void UWG_SessionBrowser::OnRefreshAllButtonClicked()
{
	WG_SessionList->SessionScrollBox->ClearChildren();
	CallFindSession();
}

void UWG_SessionBrowser::OnQuickRefreshButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

	PC_Main->ShowLogWG("Notification", "This feature is not available for now.");

	/*
	for(UWidget* LineWidget :WG_SessionList->SessionScrollBox->GetAllChildren())
	{
		UWG_SessionLine* SessionLine = Cast<UWG_SessionLine>(LineWidget);

		if(IsValid(SessionLine))
		{
			FOnlineSessionSearchResult SessionData = SessionLine->GetSessionData();
			if(SessionData.IsValid())
			{
				SessionLine->UpdateSessionData(SessionData);
			}
			else
			{
				SessionLine->RemoveFromParent();
			}
		}
		else
		{
			UE_LOG(Proto,Warning,TEXT("%s / %s : Invalid SessionLine"),*LINE_INFO,*GetNameSafe(this));
		}
	}
	*/
	/*
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub)
	{
		// 세션 인터페이스 가져오기 Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid())
		{
			ULocalPlayer* const Player = GetOwningLocalPlayer();

			TSharedPtr<const FUniqueNetId> UserID = Player->GetPreferredUniqueNetId().GetUniqueNetId();

			for(UWidget* LineWidget :WG_SessionList->SessionScrollBox->GetAllChildren())
			{
				UWG_SessionLine* SessionLine = Cast<UWG_SessionLine>(LineWidget);
				
				if(IsValid(SessionLine))
				{
					FOnlineSessionSearchResult SessionData = SessionLine->GetSessionData();
										
					FOnSingleSessionResultCompleteDelegate ResultDelegate;
					ResultDelegate.BindLambda([SessionLine](int32 LocalUserNum,bool bWasSuccessful,const FOnlineSessionSearchResult& NewData)
											  {
												  if(NewData.IsSessionInfoValid())
												  {
													  UE_LOG(Proto,Warning,TEXT("%s / Lambda : UpdateSessionData"),*LINE_INFO);
													  SessionLine->UpdateSessionData(NewData);
												  }
												  else
												  {
													  UE_LOG(Proto,Warning,TEXT("%s / Lambda : InvalidSessionData"),*LINE_INFO);
													  SessionLine->RemoveFromParent();
												  }
											  });

					Sessions->FindSessionById(*UserID.Get(),SessionData.Session.SessionInfo->GetSessionId(),*SessionData.Session.OwningUserId.Get(),ResultDelegate);

					UE_LOG(Proto,Warning,TEXT("%s / %s : Success?"),*LINE_INFO,*GetNameSafe(this));
				}
				else
				{
					UE_LOG(Proto,Warning,TEXT("%s / %s : Invalid SessionLine"),*LINE_INFO,*GetNameSafe(this));
				}
			}
		}
		else
		{
			CHECK_LOG(Sessions.IsValid());
		}
	}
	else
	{
		CHECK_LOG(OnlineSub);
	}
	]*/
}

void UWG_SessionBrowser::OnSessionCreatorButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

	functionbrake = true;

	SetVisibility(ESlateVisibility::Hidden);
	PC_Main->ShowSessionCreatorWG(3);
		
	BrakeTimer.BindLambda([this]{functionbrake = false;});
	GetWorld()->GetTimerManager().SetTimer(BrakeTimerHandle,BrakeTimer,1.0f,false);

}

void UWG_SessionBrowser::OnBacktoMainButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

	functionbrake = true;

	PC_Main->ShowMainWG(1);
	RemoveFromParent();
}

void UWG_SessionBrowser::CallFindSession()
{
	UGI_Proto* GI_Proto = Cast<UGI_Proto>(GetGameInstance());
	if(!IsValid(GI_Proto))
	{
		CHECK_LOG(!IsValid(GI_Proto));
		return;
	}
	GI_Proto->OnFindSessionReport.AddUObject(this, &UWG_SessionBrowser::OnGetFindSessionReport);

	GI_Proto->FindOnlineSession();

	SetVisibility(ESlateVisibility::HitTestInvisible);
}