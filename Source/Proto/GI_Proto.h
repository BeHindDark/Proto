// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Engine/GameInstance.h"
#include "GI_Proto.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCreateSessionReportDelegate, bool);

DECLARE_MULTICAST_DELEGATE_OneParam(FOnFindSessionReportDelegate, bool);

/**	���� �����õ��� ������ ClientTravel�� �ϱ� �� ȣ��Ǵ� ��������Ʈ
*	@param	bool	bWasSuccessful	���� ���� ���� ����
*	@param	FString	TravelURL		������ ������ URL
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
	$$���� ���� ���� ���忡 �ʿ��� ������.
	*/

	/**������ ID*/
	FName LocalUserID;

	/**���� �κ��� �� �̸�*/
	FName SessionLobyName;

public:

	/**
	$$�¶��� ���ǿ� �ʿ��� �Լ��� ����, ��������Ʈ��
	*/

	/** ������ ���� ������ ����Ǵ� �����Դϴ�.
	* �� �����ȿ� ����ִ� ���� ���� �ٲ㵵 ���� ���ǿ� �ٷ� ���� ���� �ʽ��ϴ�.
	* ������ ������ ��, Ȥ�� IOnlineSession::UpdateSession()���� ���Ǽ����� ������Ʈ ���� �� ������ ����˴ϴ�.
	* �� ������ ����ϴ� OnlineSession�� Session�� Host(dedicated�� ��쿣 server)���Ը� ������ �����ؾ��մϴ�.
	* ���Ӹ��� ���� ������ ������Ʈ�ϰų�, Host�� GameInstance_Proto�� �ִ� ���� �о� �ٸ� �������� �����ϴ� ���� �ϰ� �� ���Դϴ�.
	*/
	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	//Delegates and Handles for Create Session
	/* ���� ������ ȣ��Ǵ� ��������Ʈ Delegate called when session created */
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;

	/* ���� ���۽� ȣ��Ǵ� ��������Ʈ Delegate called when session started */
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	/** Handles to registered delegates for creating a session */
	FDelegateHandle OnCreateSessionCompleteDelegateHandle;

	/** Handles to registered delegates for starting a session */
	FDelegateHandle OnStartSessionCompleteDelegateHandle;


	//Delegate and Handle for Find Sessions
	/** ���� �˻� �Ϸ�� ȣ��Ǵ� ��������Ʈ Delegate for searching for sessions */
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;

	/** Handle to registered delegate for searching a session */
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;


	//Delagate and Handle for Join Session
	/** ���� ���� �غ�Ϸ�� ȣ��Ǵ� ��������Ʈ Delegate for joining a session */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;

	/** Handle to registered delegate for joining a session */
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;


	//Delegate and handle for Destroy Session
	/** ���� ���� �Ϸ�� ȣ��Ǵ� ��������Ʈ Delegate for destroying a session */
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;

	/** Handle to registered delegate for destroying a session */
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	FOnCreateSessionReportDelegate OnCreateSessionReport;

	FOnFindSessionReportDelegate OnFindSessionReport;

	FOnJoinSessionReportDelegate OnJoinSessionReport;

	FOnDestroySessionReportDelegate OnDestroySessionReport;

public:
	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	���ο� ������ ����� Lobby������ �̵��մϴ�.
	*	���� ���� ����� �������� OnCreateSessionReport�� �Լ��� AddUObject�� ���ε� ���ּ���.
	*	@param	SessionFrontName	����� �� UI�� ������ ������ �̸��Դϴ�.
	*	@param	HostName			������ ���� Host�� �̸��Դϴ�.
	*	@param	SessionDescription	Host�� �ۼ��� ���ǿ� ���� �����Դϴ�.
	*	@param	MaxPlayerNum		public�ϰ� ���� ������ ���� �� �Դϴ�.
	*/
	void CreateOnlineSession(FString SessionFrontName,FString HostName,FString SessionDescription,int32 MaxPlayerNum);

	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	�����ϴ� ���ǵ��� �˻��Ͽ� SessionSearch�� �����մϴ�.
	*	���� Ž�� �ϷḦ ����������� OnFindSessionReport�� �Լ��� AddUObject�� ���ε� ���ּ���.
	*/
	void FindOnlineSession();

	/**	�־��� ���ǿ� �����մϴ�.
	*	���� ���� �ϷḦ ����������� OnJoinSessionReport�� �Լ��� AddUObject�� ���ε� ���ּ���.
	*	@param	SearchResult	���� ������ ����Ű�� �˻�����Դϴ�.
	*/
	void JoinOnlineSession(const FOnlineSessionSearchResult& SearchResult);

	UFUNCTION(BlueprintCallable, Category = "Network|Session")
	/**	���� �������� ���ǿ��� �����ϴ�. Host��� ������ �ı��մϴ�.
	*	���� �ı� �ϷḦ ����������� OnDestroySessionReport�� �Լ��� AddUObject�� ���ε� ���ּ���.
	*/
	void DestroySessionAndLeaveGame();

	/**
	*	Session�� ����� Host�� �Ǳ� ���� �Լ�/Function to host a game!
	*
	*	@Param		UserID			��û�ϴ� ������ ����ID/User that started the request
	*	@Param		SessionName		�����̸�/Name of the Session
	*	@Param		bIsLAN			LAN�����ΰ�?/Is this is LAN Game?
	*	@Param		bIsPresence		���� ������ ������ Session�� ������ �ٸ� ������ �ش� ������ ID�� ���� �� �� �ִ��� �����Դϴ�.(?�Ƹ���?)"Is the Session to create a presence Session"
	*	@Param		MaxNumPlayers	�ִ� ���������� �÷��̾� ��/Number of Maximum allowed players on this "Session" (Server)
	*	@Memo		UserID�� ���ϴ� ���� : ULocalPlayer* const Player = GetFirstGamePlayer()���� Player->GetPreferredUniqueNetID();�̴�.
	*/
	bool HostSession(TSharedPtr<const FUniqueNetId> UserId,FName SessionName,bool bIsLAN,bool bIsPresence,int32 MaxNumPlayers,FString SessionFrontName,FString HostUserID,FString SessionDescription);

	/**
	*	Session���� ��û�� �Ϸ�Ǿ��� �� ����Ǵ� ��������Ʈ�Լ�/Function fired when a session create request has completed
	*
	*	@param SessionName		�ش��ϴ� ���� �̸�/the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	Session���� ��û�� �Ϸ�Ǿ��� �� ����Ǵ� ��������Ʈ�Լ�/Function fired when a session start request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);

	/**
	*	���� �˻�/Find an online session
	*
	*	@param UserId		��û�ϴ� ������ ����ID/user that initiated the request
	*	@param bIsLAN		LAN������ �˻����� ����/are we searching LAN matches
	*	@param bIsPresence	are we searching presence sessions
	*/
	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	/**
	*	���� �˻��� ������ �� ȣ��Ǵ� ��������Ʈ�Լ�/Delegate fired when a session search query has completed
	*
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnFindSessionsComplete(bool bWasSuccessful);

	/**
	*	Joins a session via a search result
	*
	*	@param SessionName	������ ������ �̸�/name of session
	*	@param SearchResult	������ ������ �˷��� �˻����/Session to join
	*
	*	@return bool		��������/true if successful, false otherwise
	*/
	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	/**
	*	���� ������û�� �Ϸ�Ǿ��� �� ����Ǵ� ��������Ʈ�Լ�/Delegate fired when a session join request has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	/**
	*	���� �������� ���ǿ��� �����ϴ�.
	*	Ŭ���̾�Ʈ�� �����ϸ� ���ǿ��� ������, Host�� �����ϸ� ������ �ı��Ǵ� ���� �����ϱ� �ٶ��ϴ�.
	*/
	void LeaveCurrentSession();

	/**
	*	�¶��� ���� �ı� ��û�� �Ϸ�Ǿ��� �� ����Ǵ� ��������Ʈ�Լ�/Delegate fired when a destroying an online session has completed
	*
	*	@param SessionName the name of the session this callback is for
	*	@param bWasSuccessful true if the async action completed without error, false if there was an error
	*/
	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
};