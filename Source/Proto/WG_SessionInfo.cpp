// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionInfo.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableText.h"
#include "Components/SizeBox.h"

void UWG_SessionInfo::NativeConstruct()
{
	Super::NativeConstruct();
	SetIsEditable(bIsEditable);
	ClearText();
}

void UWG_SessionInfo::SetIsEditable(bool Editable)
{
	bIsEditable = Editable;

	if(bIsEditable)
	{
		SessionNameTextBox->SetIsReadOnly(false);
		DescriptionMLEditableText->SetIsReadOnly(false);
		HostUserIDTextBox->SetIsReadOnly(true);
		HostUserIDTextBox->SetText(FText::FromString(FString(TEXT("textsettingtest, hostname"))));
		ScrollSize->SetHeightOverride(500.0f);
	}
	else
	{
		SessionNameTextBox->SetIsReadOnly(true);
		DescriptionMLEditableText->SetIsReadOnly(true);
		HostUserIDTextBox->SetIsReadOnly(true);
		HostUserIDTextBox->SetText(FText::FromString(FString(TEXT("textsettingtest, hostname"))));
		ScrollSize->SetHeightOverride(200.0f);
	}
}

void UWG_SessionInfo::ClearText()
{
	if(!bIsEditable)
	{
		HostUserIDTextBox->SetText(FText::FromString(""));
	}
	SessionNameTextBox->SetText(FText::FromString(""));
	DescriptionMLEditableText->SetText(FText::FromString(""));
}

bool UWG_SessionInfo::UpdateSessionData(const FOnlineSessionSearchResult & NewSessionData)
{
	if(NewSessionData.IsSessionInfoValid())
	{
		SessionData = NewSessionData;
		//SessionData.GetSessionIdStr();

		//유저에게 보여줄 세션 이름
		if(SessionData.Session.SessionSettings.Settings.Contains(FName("SessionFrontName")))
		{
			FString SessionNameString;
			SessionData.Session.SessionSettings.Settings[FName("SessionFrontName")].Data.GetValue(SessionNameString);
			SessionNameTextBox->SetText(FText::FromString(SessionNameString));
		}
		else
		{
			SessionNameTextBox->SetText(FText::FromString(TEXT("None")));
		}

		//SessionHost 이름
		if(SessionData.Session.SessionSettings.Settings.Contains(FName("HostUserID")))
		{
			FString HostUserIDString;
			SessionData.Session.SessionSettings.Settings[FName("HostUserID")].Data.GetValue(HostUserIDString);
			HostUserIDTextBox->SetText(FText::FromString(HostUserIDString));
		}
		else
		{
			HostUserIDTextBox->SetText(FText::FromString(TEXT("None")));
		}

		//Session Description 세션 세부 설명
		if(SessionData.Session.SessionSettings.Settings.Contains(FName("SessionDescription")))
		{
			FString DescriptionString;
			SessionData.Session.SessionSettings.Settings[FName("SessionDescription")].Data.GetValue(DescriptionString);
			DescriptionMLEditableText->SetText(FText::FromString(DescriptionString));
		}
		else
		{
			DescriptionMLEditableText->SetText(FText::FromString(TEXT("None")));
		}

		//맵 이름
		/*
		FString MapString;
		if(SessionData.Session.SessionSettings.Get(SETTING_MAPNAME,MapString))
		{
			MapText->SetText(FText::FromString(MapString));
		}
		else
		{
			MapText->SetText(FText::FromString(TEXT("None")));
		}
		*/

		//모드 이름
		/*
		FString ModeString;
		if(SessionData.Session.SessionSettings.Get(SETTING_GAMEMODE,ModeString))
		{
			ModeText->SetText(FText::FromString(ModeString));
		}
		else
		{
			ModeText->SetText(FText::FromString(TEXT("None")));
		}
		*/

		// CurrentPlayer/MaxPlayer 설정
		/*
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
		*/

		// Ping 설정
		//PingText->SetText(FText::FromString(FString::FromInt(SessionData.PingInMs)+FString("ms")));

		return true;
	}
	else
	{
		ClearText();
		SessionNameTextBox->SetText(FText::FromString(TEXT("InValidSession")));
		//MapText->SetText(FText::FromString(TEXT("")));
		//ModeText->SetText(FText::FromString(TEXT("")));
		//CurrentPlayerText->SetText(FText::FromString(TEXT("")));
		//PlayerSeatText->SetText(FText::FromString(TEXT("")));
		//MaxPlayerText->SetText(FText::FromString(TEXT("")));
		//PingText->SetText(FText::FromString(TEXT("")));

		return false;
	}
}
