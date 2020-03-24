// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Proto.h"

UGI_Proto::UGI_Proto(const FObjectInitializer& ObjectInitializer) {
	// 초기화

	SessionLobyName = TEXT("SessionLobyMap");

	/** 세션에 관련된 함수와 델리게이트를 바인딩한다. */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &UGI_Proto::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &UGI_Proto::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &UGI_Proto::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &UGI_Proto::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &UGI_Proto::OnDestroySessionComplete);
}
void UGI_Proto::CreateOnlineSession(FString SessionFrontName,FString HostName,FString SessionDescription,int32 MaxPlayerNum)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();
	
	HostSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(), GameSessionName, false, true, MaxPlayerNum, SessionFrontName, HostName, SessionDescription);
}

void UGI_Proto::FindOnlineSession()
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	FindSessions(Player->GetPreferredUniqueNetId().GetUniqueNetId(), false, true);
}

void UGI_Proto::JoinOnlineSession(const FOnlineSessionSearchResult & SearchResult)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	if(!JoinSession(Player->GetPreferredUniqueNetId().GetUniqueNetId(),GameSessionName,SearchResult))
	{
		OnJoinSessionReport.Broadcast(false, "");
	}
}

void UGI_Proto::DestroySessionAndLeaveGame()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			Sessions->DestroySession(GameSessionName);
		}
	}
}

bool UGI_Proto::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLAN, bool bIsPresence, int32 MaxNumPlayers, FString SessionFrontName, FString HostName, FString SessionDescription) {

	// Creating a local player where we can get the UserID from
	//ULocalPlayer* const Player = GetFirstGamePlayer();

	// Call our custom HostSession function. GameSessionName is a GameInstance variable
	//HostSession(Player->GetPreferredUniqueNetId(),GameSessionName,true,true,4);

	//return bool
	bool bSuccess = false;

	// Online Subsystem 불러오기
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// 세션 인터페이스 가져오기 Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			
			SessionSettings = MakeShareable(new FOnlineSessionSettings());

			SessionSettings->bIsLANMatch = bIsLAN;
			SessionSettings->bUsesPresence = bIsPresence;
			SessionSettings->NumPublicConnections = MaxNumPlayers;
			SessionSettings->NumPrivateConnections = 0;
			SessionSettings->bAllowInvites = true;
			SessionSettings->bAllowJoinInProgress = true;
			SessionSettings->bShouldAdvertise = true;
			SessionSettings->bAllowJoinViaPresence = true;
			SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

			//커스텀 세팅 추가
			SessionSettings->Settings.Add(FName("SessionFrontName"),FOnlineSessionSetting(SessionFrontName, EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName("bIsPrivate"), FOnlineSessionSetting(false, EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName("HostUserID"), FOnlineSessionSetting(HostName, EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName("SessionDescription"), FOnlineSessionSetting(SessionDescription, EOnlineDataAdvertisementType::ViaOnlineService));

			//거꾸로 세션 세팅을 받아갈때는 아래와 같이 합니다.
			//bool val;
			//SessionSettings->Settings[FName("bIsPrivate")].Data.GetValue(val);
			//맵 이름
			SessionSettings->Set(SETTING_MAPNAME, SessionLobyName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			//게임 모드 이름
			SessionSettings->Set(SETTING_GAMEMODE, FString(TEXT("Default GameMode")), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			bSuccess = Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, TEXT("No OnlineSubsystem found!"));
	}

	return bSuccess;
}

void UGI_Proto::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnCreateSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// 핸들을 통해 델리게이트 해제 Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if (bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);

				return;
			}
		}
	}

	OnCreateSessionReport.Broadcast(bWasSuccessful);

	//실패한 경우는 HostSession함수가 이미 false를 return했을 것이니 그쪽에서 처리한다.
}

void UGI_Proto::OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// 핸들을 통해 델리게이트 해제 Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	//적절하 GM, Pcon, WG에 완료 결과를 보고한다.
	//성공시 적절한 레벨을 열고, 실패 시엔 실패위젯을 띄운다.
	OnCreateSessionReport.Broadcast(bWasSuccessful);

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	/*
	if(bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), SessionLobyName,true,"listen");
	}
	*/
}

void UGI_Proto::FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence) {
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 100;
			SessionSearch->PingBucketSize = 250;

			// We only want to set this Query Setting if "bIsPresence" is true
			if (bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bIsPresence, EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef();

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId, SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UGI_Proto::OnFindSessionsComplete(bool bWasSuccessful) {
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));

			// 여기서 ReportDelegate를 broadcast하여 위젯등에게 검색이 완료되었음을 알리고 적절한 logic을 실행하게 합니다.
			// 검색 완료된 세션들은 GI의 SessionSearch->SearchResults 배열에 저장됩니다.
			// 성공시엔 위젯에 세션 목록을 띄우고, 실패시엔 에러위젯을 띄웁니다.
			OnFindSessionReport.Broadcast(bWasSuccessful);

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if (SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for (int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIdx + 1, *(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}
}


bool UGI_Proto::JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	// Return bool
	bool bSuccess = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid() && UserId.IsValid())
		{
			// 델리게이트 핸들 재설정 Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);

			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccess = Sessions->JoinSession(*UserId, SessionName, SearchResult);
		}
	}

	return bSuccess;
}

void UGI_Proto::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnJoinSessionComplete %s, %d"), *SessionName.ToString(), static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// 핸들 해제 Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);


			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController* const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if (Sessions->GetResolvedConnectString(SessionName, TravelURL))
			{
				//이 아랫부분에서 실제로 Client Travel이 일어납니다.
				//이 부분에서 ReportDelegate를 호출하고
				//해당 델리게이트는 성공여부를 parameter로 전달해야 합니다.
				//성공여부가 true면 서버탐색기 위젯을 제거해주고
				//fail이면 로딩스크린을 닫고, 서버탐색기 위젯을 다시 보여주며, 에러위젯을 띄워줍니다.
				OnJoinSessionReport.Broadcast(true, TravelURL);

			}
			else
			{
				OnJoinSessionReport.Broadcast(false, FString(""));
			}
		}
	}
}

void UGI_Proto::LeaveCurrentSession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			//GameSessionName은 GameInstance의 변수입니다.
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UGI_Proto::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if (Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			// If it was successful, we just load another level (could be a MainMenu!)
			if (bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(), "mainmenumap", true);
			}
			//Sessions->UnregisterPlayer();
			//Sessions->DestroySession();
			//AGameSession::UnregisterPlayer();
		}
	}

}