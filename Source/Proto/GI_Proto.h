// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Engine/GameInstance.h"
#include "GI_Proto.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateSessionReportDelegate, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindSessionReportDelegate, bool);

/**	세션 참가시도가 끝나고 ClientTravel을 하기 전 호출되는 델리게이트
*	@param	bool	bWasSuccessful	세션 참가 성공 여부
*	@param	FString	TravelURL		참가할 세션의 URL
*/
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnJoinSessionReportDelegate, bool, FString);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnDestroySessionReportDelegate, bool);

/**
 *
 */
UCLASS()
class PROTO_API UGI_Proto : public UGameInstance
{
	GENERATED_BODY()

public:
	UGI_Proto(const FObjectInitializer& ObjectInitializer);

	/**
	$$각종 게임 정보 저장에 필요한 변수들.
	*/

	/**유저의 ID*/
	FName LocalUserID;

	/**세션 로비의 맵 이름*/
	FName SessionLobyName;

public:

	/**
	$$온라인 세션에 필요한 함수와 변수, 델리게이트들
	*/

	/** 세션의 각종 세팅이 저장되는 변수입니다.
	* 이 변수안에 들어있는 각종 값을 바꿔도 실제 세션엔 바로 적용 되진 않습니다.
	* 세션을 생성할 때, 혹은 IOnlineSession::UpdateSession()으로 세션설정을 업데이트 했을 떄 실제로 적용됩니다.
	* 이 세팅을 사용하는 OnlineSession은 Session의 Host(dedicated인 경우엔 server)에게만 있으니 주의해야합니다.
	* 게임모드는 세션 정보를 업데이트하거나, Host의 GameInstance_Proto에 있는 값을 읽어 다른 유저에게 전파하는 일을 하게 될 것입니다.
	*/
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//Delegates and Handles for Create Session
	/* 세션 생성시 호출되는 델리게이트 Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	/* 세션 시작시 호출되는 델리게이트 Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/** Handles to registered delegates for starting a session */
	FDelegateHandle OnStartSessionCompleteDelegateHandle;


	//Delegate and Handle for Find Sessions
	/** 세션 검색 완료시 호출되는 델리게이트 Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;


	//Delagate and Handle for Join Session
	/** 세션 참가 준비완료시 호출되는 델리게이트 Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;


	//Delegate and handle for Destroy Session
	/** 세션 종료 완료시 호출되는 델리게이트 Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	FOnCreateSessionReportDelegate OnCreateSessionReport;

	FOnFindSessionReportDelegate OnFindSessionReport;

	FOnJoinSessionReportDelegate OnJoinSessionReport;

	FOnDestroySessionReportDelegate OnDestroySessionReport;

public:
	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	새로운 세션을 만들고 Lobby맵으로 이동합니다.
	*	세션 생성 결과를 받으려면 OnCreateSessionReport에 함수를 AddUObject로 바인드 해주세요.
	*	@param	SessionFrontName	사용자 및 UI에 보여질 세션의 이름입니다.
	*	@param	HostName			세션을 만든 Host의 이름입니다.
	*	@param	SessionDescription	Host가 작성한 세션에 대한 설명입니다.
	*	@param	MaxPlayerNum		public하게 참가 가능한 유저 수 입니다.
	*/
	void CreateOnlineSession(FString SessionFrontName,FString HostName,FString SessionDescription,int32 MaxPlayerNum);

	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	존재하는 세션들을 검색하여 SessionSearch에 저장합니다.
	*	세션 탐색 완료를 보고받으려면 OnFindSessionReport에 함수를 AddUObject로 바인드 해주세요.
	*/
	void FindOnlineSession();

	/**	주어진 세션에 참가합니다.
	*	세션 참가 완료를 보고받으려면 OnJoinSessionReport에 함수를 AddUObject로 바인드 해주세요.
	*	@param	SearchResult	단일 세션을 가리키는 검색결과입니다.
	*/
	void JoinOnlineSession(const FOnlineSessionSearchResult& SearchResult);

	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	현재 참가중인 세션에서 나갑니다. Host라면 세션을 파괴합니다.
	*	세션 파괴 완료를 보고받으려면 OnDestroySessionReport에 함수를 AddUObject로 바인드 해주세요.
	*/
	void DestroySessionAndLeaveGame();

	/**
	*	Session을 만들어 Host가 되기 위한 함수/Function to host a game!
	*
	*	@Param		UserID			요청하는 유저의 고유ID/User that started the request
	*	@Param		SessionName		세션이름/Name of the Session
	*	@Param		bIsLAN			LAN게임인가?/Is this is LAN Game?
	*	@Param		bIsPresence		현재 유저가 참가한 Session의 정보를 다른 유저가 해당 유저의 ID를 통해 볼 수 있는지 여부입니다.(?아마도?)"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	최대 참가가능한 플레이어 수/Number of Maximum allowed players on this "Session" (Server)
	*	@Memo		UserID를 구하는 법은 : ULocalPlayer* const Player = GetFirstGamePlayer()에서 Player->GetPreferredUniqueNetID();이다.
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,bool bIsLAN,bool bIsPresence,int32 MaxNumPlayers,FString SessionFrontName,FString HostUserID,FString SessionDescription);

	/**
	*	Session생성 요청이 완료되었을 때 실행되는 델리게이트함수/Function fired when a session create request has completed
	*
	*	@param SessionName		해당하는 세션 이름/the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Session시작 요청이 완료되었을 때 실행되는 델리게이트함수/Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	세션 검색/Find an online session
	*
	*	@param UserId		요청하는 유저의 고유ID/user that initiated the request
	*	@param bIsLAN		LAN게임을 검색할지 여부/are we searching LAN matches
	*	@param bIsPresence	are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/**
	*	세션 검색이 끝났을 때 호출되는 델리게이트함수/Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	*	Joins a session via a search result
	*
	*	@param SessionName	참가할 세션의 이름/name of session
	*	@param SearchResult	참가할 세션을 알려줄 검색결과/Session to join
	*
	*	@return bool		성공여부/true if successful, false otherwise
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/**
	*	세션 참가요청이 완료되었을 때 실행되는 델리게이트함수/Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	*	현재 참가중인 세션에서 나갑니다.
	*	클라이언트가 실행하면 세션에서 나가고, Host가 실행하면 세션이 파괴되니 사용시 주의하기 바랍니다.
	*/
	void LeaveCurrentSession();

	/**
	*	온라인 세션 파괴 요청이 완료되었을 때 실행되는 델리게이트함수/Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};