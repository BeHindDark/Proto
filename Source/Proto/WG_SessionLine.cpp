// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionLine.h"
#include "WG_SessionList.h"
#include "WG_SessionBrowser.h"
#include "WG_SessionInfo.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UWG_SessionLine::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(bIsTitle)
	{
		//배경을 살짝 어둡게
		//BackgroundBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		SessionNameText->SetText(FText::FromString(TEXT("Session Name")));
		MapText->SetText(FText::FromString(TEXT("Map")));
		ModeText->SetText(FText::FromString(TEXT("Mode")));
		CurrentPlayerText->SetText(FText::FromString(TEXT("")));
		PlayerSeatText->SetText(FText::FromString(TEXT("Player Seat")));
		MaxPlayerText->SetText(FText::FromString(TEXT("")));
		PingText->SetText(FText::FromString(TEXT("Ping")));
		CurrentBoxColor = TitleBoxColor;
	}
	else
	{
		CurrentBoxColor = NormalBoxColor;
		PlayerSeatText->SetText(FText::FromString(TEXT("/")));
	}
}

void UWG_SessionLine::NativeOnMouseEnter(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	if(!bIsTitle)
	{
		BackgroundBorder->SetBrushColor(HoveredBackgroundColor);	
	}
	
}

void UWG_SessionLine::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if(!bIsTitle)
	{
		//배경을 투명하게
		if(bIsSelected)
		{
			BackgroundBorder->SetBrushColor(SelectedBackgroundColor);
		}
		else
		{
			BackgroundBorder->SetBrushColor(NormalBackgroundColor);
		}
	}
	
}


FReply UWG_SessionLine::NativeOnMouseButtonDown(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if(bIsTitle) return FReply::Unhandled();

	if(!SessionData.IsValid()) return FReply::Unhandled();

	if(!SessionBrowser_Ref.IsValid()) return FReply::Unhandled();

	//다른 선택된 위젯을 해제합니다.
	SessionBrowser_Ref->WG_SessionList->ClearSelection();

	//여기서 위젯의 색상이 변경됩니다.
	SetSelect(true);	

	//여기서 SessionBrowser의 함수를 호출합니다.
	
	return FReply::Unhandled();
}

FReply UWG_SessionLine::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	
	return FReply::Unhandled();
}

void UWG_SessionLine::SetSelect(bool Selected)
{
	bIsSelected = Selected;
	if(bIsSelected)
	{
		CurrentBoxColor = TitleBoxColor;
		BackgroundBorder->SetContentColorAndOpacity(SelectedContentColor);
		if(IsHovered())
		{
			BackgroundBorder->SetBrushColor(HoveredBackgroundColor);
		}
		else
		{
			BackgroundBorder->SetBrushColor(SelectedBackgroundColor);
		}
	}
	else
	{
		CurrentBoxColor = NormalBoxColor;
		BackgroundBorder->SetContentColorAndOpacity(NormalContentColor);
		if(IsHovered())
		{
			BackgroundBorder->SetBrushColor(HoveredBackgroundColor);
		}
		else
		{
			BackgroundBorder->SetBrushColor(NormalBackgroundColor);
		}		
	}
}

bool UWG_SessionLine::GetIsSelected()
{
	return bIsSelected;
}

bool UWG_SessionLine::UpdateSessionData(FOnlineSessionSearchResult NewSession)
{
	if(NewSession.IsSessionInfoValid())
	{
		SessionData = NewSession;
		//SessionData.GetSessionIdStr();
		
		//유저에게 보여줄 세션 이름
		if(SessionData.Session.SessionSettings.Settings.Contains(FName("SessionFrontName")))
		{
			FString SessionNameString;
			SessionData.Session.SessionSettings.Settings[FName("SessionFrontName")].Data.GetValue(SessionNameString);
			SessionNameText->SetText(FText::FromString(SessionNameString));
		}
		else
		{
			SessionNameText->SetText(FText::FromString(TEXT("None")));
		}

		//맵 이름
		FString MapString;
		if(SessionData.Session.SessionSettings.Get(SETTING_MAPNAME,MapString))
		{
			MapText->SetText(FText::FromString(MapString));
		}
		else
		{
			MapText->SetText(FText::FromString(TEXT("None")));
		}
		
		//모드 이름
		FString ModeString;
		if(SessionData.Session.SessionSettings.Get(SETTING_GAMEMODE, ModeString))
		{
			ModeText->SetText(FText::FromString(ModeString));
		}
		else
		{
			ModeText->SetText(FText::FromString(TEXT("None")));
		}
		
		// CurrentPlayer/MaxPlayer 설정
		if(SessionData.Session.SessionSettings.NumPublicConnections > 0)
		{
			CurrentPlayerText->SetText(FText::FromString(FString::FromInt(SessionData.Session.SessionSettings.NumPublicConnections - SessionData.Session.NumOpenPublicConnections)));
			MaxPlayerText->SetText(FText::FromString(FString::FromInt(SessionData.Session.SessionSettings.NumPublicConnections)));
		}
		else
		{
			CurrentPlayerText->SetText(FText::FromString(TEXT("None")));
			MaxPlayerText->SetText(FText::FromString(TEXT("None")));
		}
		
		PlayerSeatText->SetText(FText::FromString(TEXT("/")));
		
		// Ping 설정
		PingText->SetText(FText::FromString(FString::FromInt(SessionData.PingInMs)+FString("ms")));

		return true;
	}
	else
	{
		SessionNameText->SetText(FText::FromString(TEXT("InValidSession")));
		MapText->SetText(FText::FromString(TEXT("")));
		ModeText->SetText(FText::FromString(TEXT("")));
		CurrentPlayerText->SetText(FText::FromString(TEXT("")));
		PlayerSeatText->SetText(FText::FromString(TEXT("")));
		MaxPlayerText->SetText(FText::FromString(TEXT("")));
		PingText->SetText(FText::FromString(TEXT("")));

		return false;
	}
}

FOnlineSessionSearchResult UWG_SessionLine::GetSessionData()
{
	return SessionData;
}

void UWG_SessionLine::ConnectBrowser(UWG_SessionBrowser * SessionBrowser)
{
	SessionBrowser_Ref = TWeakObjectPtr<UWG_SessionBrowser>(SessionBrowser);
}
