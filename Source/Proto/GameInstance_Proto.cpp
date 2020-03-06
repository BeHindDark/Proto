// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance_Proto.h"

UGameInstance_Proto::UGameInstance_Proto(const FObjectInitializer& ObjectInitializer)
{
	// �ʱ�ȭ

	SessionLobyName = TEXT("SessionLobyMap");

	/** ���ǿ� ���õ� �Լ��� ��������Ʈ�� ���ε��Ѵ�. */
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this,&UGameInstance_Proto::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this,&UGameInstance_Proto::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this,&UGameInstance_Proto::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this,&UGameInstance_Proto::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this,&UGameInstance_Proto::OnDestroySessionComplete);
	
}

bool UGameInstance_Proto::HostSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,bool bIsLAN,bool bIsPresence,int32 MaxNumPlayers)
{
	//return bool
	bool bSuccess = false;

	// Online Subsystem �ҷ�����
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub)
	{
		// ���� �������̽� �������� Get the Session Interface, so we can call the "CreateSession" function on it
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid())
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

			//Ŀ���� ���� �߰�
			SessionSettings->Settings.Add(FName("bIsPrivate"), FOnlineSessionSetting(false, EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName("HostUserID"), FOnlineSessionSetting(FString(TEXT("userid")), EOnlineDataAdvertisementType::ViaOnlineService));
			SessionSettings->Settings.Add(FName("SessionDescription"),FOnlineSessionSetting(FString(TEXT("sessiondescription")), EOnlineDataAdvertisementType::ViaOnlineService));

			//�Ųٷ� ���� ������ �޾ư����� �Ʒ��� ���� �մϴ�.
			//bool val;
			//SessionSettings->Settings[FName("bIsPrivate")].Data.GetValue(val);
			//�� �̸�
			SessionSettings->Set(SETTING_MAPNAME, SessionLobyName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

			//���� ��� �̸�
			SessionSettings->Set(SETTING_GAMEMODE, FString(TEXT("Default GameMode")), EOnlineDataAdvertisementType::ViaOnlineService);

			// Set the delegate to the Handle of the SessionInterface
			OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);

			// Our delegate should get called when this is complete (doesn't need to be successful!)
			bSuccess =  Sessions->CreateSession(*UserId,SessionName,*SessionSettings);
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,TEXT("No OnlineSubsytem found!"));
	}

	return bSuccess;
}

void UGameInstance_Proto::OnCreateSessionComplete(FName SessionName,bool bWasSuccessful)
{

	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("OnCreateSessionComplete %s, %d"),*SessionName.ToString(),bWasSuccessful));

	// Get the OnlineSubsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		// Get the Session Interface to call the StartSession function
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid())
		{
			// �ڵ��� ���� ��������Ʈ ���� Clear the SessionComplete delegate handle, since we finished this call
			Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
			if(bWasSuccessful)
			{
				// Set the StartSession delegate handle
				OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);

				// Our StartSessionComplete delegate should get called after this
				Sessions->StartSession(SessionName);
			}
		}
	}
	//������ ���� HostSession�Լ��� �̹� false�� return���� ���̴� ���ʿ��� ó���Ѵ�.
}

void UGameInstance_Proto::OnStartOnlineGameComplete(FName SessionName,bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("OnStartSessionComplete %s, %d"),*SessionName.ToString(),bWasSuccessful));

	// Get the Online Subsystem so we can get the Session Interface
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		// Get the Session Interface to clear the Delegate
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if(Sessions.IsValid())
		{
			// �ڵ��� ���� ��������Ʈ ���� Clear the delegate, since we are done with this call
			Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);
		}
	}

	//������ GM, Pcon, WG�� �Ϸ� ����� �����Ѵ�.
	//������ ������ ������ ����, ���� �ÿ� ���������� ����.
	OnCreateSessionReport.Broadcast(bWasSuccessful);

	// If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter!
	/*
	if(bWasSuccessful)
	{
		UGameplayStatics::OpenLevel(GetWorld(), SessionLobyName,true,"listen");
	}
	*/
}

void UGameInstance_Proto::FindSessions(TSharedPtr<const FUniqueNetId> UserId,bool bIsLAN,bool bIsPresence)
{
	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub)
	{
		// Get the SessionInterface from our OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid())
		{
			/*
				Fill in all the SearchSettings, like if we are searching for a LAN game and how many results we want to have!
			*/
			SessionSearch = MakeShareable(new FOnlineSessionSearch());

			SessionSearch->bIsLanQuery = bIsLAN;
			SessionSearch->MaxSearchResults = 20;
			SessionSearch->PingBucketSize = 50;

			// We only want to set this Query Setting if "bIsPresence" is true
			if(bIsPresence)
			{
				SessionSearch->QuerySettings.Set(SEARCH_PRESENCE,bIsPresence,EOnlineComparisonOp::Equals);
			}

			TSharedRef<FOnlineSessionSearch> SearchSettingsRef = SessionSearch.ToSharedRef(); 

			// Set the Delegate to the Delegate Handle of the FindSession function
			OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);

			// Finally call the SessionInterface function. The Delegate gets called once this is finished
			Sessions->FindSessions(*UserId,SearchSettingsRef);
		}
	}
	else
	{
		// If something goes wrong, just call the Delegate Function directly with "false".
		OnFindSessionsComplete(false);
	}
}

void UGameInstance_Proto::OnFindSessionsComplete(bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"),bWasSuccessful));

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		// Get SessionInterface of the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if(Sessions.IsValid())
		{
			// Clear the Delegate handle, since we finished this call
			Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);

			// Just debugging the Number of Search results. Can be displayed in UMG or something later on
			GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("Num Search Results: %d"),SessionSearch->SearchResults.Num()));

			// ���⼭ ReportDelegate�� broadcast�Ͽ� ������� �˻��� �Ϸ�Ǿ����� �˸��� ������ logic�� �����ϰ� �մϴ�.
			// �˻� �Ϸ�� ���ǵ��� GI�� SessionSearch->SearchResults �迭�� ����˴ϴ�.
			// �����ÿ� ������ ���� ����� ����, ���нÿ� ���������� ���ϴ�.
			OnFindSessionReport.Broadcast(bWasSuccessful);

			// If we have found at least 1 session, we just going to debug them. You could add them to a list of UMG Widgets, like it is done in the BP version!
			if(SessionSearch->SearchResults.Num() > 0)
			{
				// "SessionSearch->SearchResults" is an Array that contains all the information. You can access the Session in this and get a lot of information.
				// This can be customized later on with your own classes to add more information that can be set and displayed
				for(int32 SearchIdx = 0; SearchIdx < SessionSearch->SearchResults.Num(); SearchIdx++)
				{
					// OwningUserName is just the SessionName for now. I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
					// This is something you can't do in Blueprint for example!
					GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("Session Number: %d | Sessionname: %s "),SearchIdx+1,*(SessionSearch->SearchResults[SearchIdx].Session.OwningUserName)));
				}
			}
		}
	}
}

bool UGameInstance_Proto::JoinSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,const FOnlineSessionSearchResult & SearchResult)
{
	// Return bool
	bool bSuccess = false;

	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();

	if(OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid() && UserId.IsValid())
		{
			// ��������Ʈ �ڵ� �缳�� Set the Handle again
			OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
			
			// Call the "JoinSession" Function with the passed "SearchResult". The "SessionSearch->SearchResults" can be used to get such a
			// "FOnlineSessionSearchResult" and pass it. Pretty straight forward!
			bSuccess = Sessions->JoinSession(*UserId,SessionName,SearchResult);
		}
	}

	return bSuccess;
}

void UGameInstance_Proto::OnJoinSessionComplete(FName SessionName,EOnJoinSessionCompleteResult::Type Result)
{
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("OnJoinSessionComplete %s, %d"),*SessionName.ToString(),static_cast<int32>(Result)));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid())
		{
			// �ڵ� ���� Clear the Delegate again
			Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);

			
			// Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map
			// This is something the Blueprint Node "Join Session" does automatically!
			APlayerController * const PlayerController = GetFirstLocalPlayerController();

			// We need a FString to use ClientTravel and we can let the SessionInterface contruct such a
			// String for us by giving him the SessionName and an empty String. We want to do this, because
			// Every OnlineSubsystem uses different TravelURLs
			FString TravelURL;

			if(PlayerController && Sessions->GetResolvedConnectString(SessionName,TravelURL))
			{
				//�� �Ʒ��κп��� ������ Client Travel�� �Ͼ�ϴ�.
				//�� �κп��� ReportDelegate�� ȣ���ϰ�
				//�ش� ��������Ʈ�� �������θ� parameter�� �����ؾ� �մϴ�.
				//�������ΰ� true�� ����Ž���� ������ �������ְ�
				//fail�̸� �ε���ũ���� �ݰ�, ����Ž���� ������ �ٽ� �����ָ�, ���������� ����ݴϴ�.
				OnJoinSessionReport.Broadcast(true);


				// Finally call the ClienTravel. If you want, you could print the TravelURL to see
				// how it really looks like
				PlayerController->ClientTravel(TravelURL,ETravelType::TRAVEL_Absolute);
			}
			else
			{
				OnJoinSessionReport.Broadcast(false);
			}
		}
	}
}

void UGameInstance_Proto::LeaveCurrentSession()
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();

		if(Sessions.IsValid())
		{
			Sessions->AddOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegate);

			//GameSessionName�� GameInstance�� �����Դϴ�.
			Sessions->DestroySession(GameSessionName);
		}
	}
}

void UGameInstance_Proto::OnDestroySessionComplete(FName SessionName,bool bWasSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1,10.f,FColor::Red,FString::Printf(TEXT("OnDestroySessionComplete %s, %d"),*SessionName.ToString(),bWasSuccessful));

	// Get the OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if(OnlineSub)
	{
		// Get the SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		
		if(Sessions.IsValid())
		{
			// Clear the Delegate
			Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
			// If it was successful, we just load another level (could be a MainMenu!)
			if(bWasSuccessful)
			{
				UGameplayStatics::OpenLevel(GetWorld(),"mainmenumap",true);
			}
			//Sessions->UnregisterPlayer();
			//Sessions->DestroySession();
			//AGameSession::UnregisterPlayer();
		}
	}
	
}
