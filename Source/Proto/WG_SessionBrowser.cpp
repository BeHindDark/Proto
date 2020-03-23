// Fill out your copyright notice in the Description page of Project Settings.


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
	QuickRefreshButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnQuickRefreshButtonClicked);
	SessionCreatorButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnSessionCreatorButtonClicked);
	BacktoMainButton->OnClicked.AddDynamic(this,&UWG_SessionBrowser::OnBacktoMainButtonClicked);

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
}

void UWG_SessionBrowser::OnRefreshAllButtonClicked()
{
	WG_SessionList->SessionScrollBox->ClearChildren();
	CallFindSession();
}

void UWG_SessionBrowser::OnQuickRefreshButtonClicked()
{
	for(UWidget* LineWidget :WG_SessionList->SessionScrollBox->GetAllChildren())
	{
		UWG_SessionLine* SessionLine = Cast<UWG_SessionLine>(LineWidget);
		if(IsValid(SessionLine))
		{
			FOnlineSessionSearchResult SessionData = SessionLine->GetSessionData();
			SessionLine->UpdateSessionData(SessionData);
		}
	}
}

void UWG_SessionBrowser::OnSessionCreatorButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

	SetVisibility(ESlateVisibility::Hidden);
	PC_Main->ShowSessionCreatorWG(3);

}

void UWG_SessionBrowser::OnBacktoMainButtonClicked()
{
	APC_Main* PC_Main = Cast<APC_Main>(GetOwningPlayer());
	if(!IsValid(PC_Main))
	{
		CHECK_LOG(!IsValid(PC_Main));
		return;
	}

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