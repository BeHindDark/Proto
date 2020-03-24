// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/PlayerController.h"
#include "PC_Main.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API APC_Main : public APlayerController
{
	GENERATED_BODY()

	APC_Main();

protected:
	virtual void BeginPlay() override;



public:
	class UWG_Main* WG_Main;

	class UWG_SessionBrowser* WG_SessionBrowser;

	class UWG_SessionCreator* WG_SessionCreator;

	class UUserWidget* WG_LoadingScreen;


protected:

	TSubclassOf<class UWG_Main> WG_Main_Class;

	TSubclassOf<class UWG_SessionBrowser> WG_SessionBrowser_Class;

	TSubclassOf<class UWG_SessionCreator> WG_SessionCreator_Class;

	TSubclassOf<class UUserWidget> WG_LoadingScreen_Class;

public:

	void ShowMainWG(int Zorder = 0);

	void ShowSessionBrowserWG(int Zorder = 0);

	void ShowSessionCreatorWG(int Zorder = 0);

	void ShowLoadingScreenWG(int Zorder = 0);

	/**	���� ������ ����, �Ǵ� �������� �� ����˴ϴ�.
	*	@param	bWasSuccessful	���� ���� ���������Դϴ�.
	*			==true	�κ������ �̵��մϴ�.
	*			==false	�ε���ũ�� ������ �ݰ� ���ǻ��� �������� ���ư��ϴ�.
	*	@detail	WG_SessionCreator::OnGetCreateSessionReport(bool)�Լ����� ȣ��˴ϴ�.
	*/
	void OnGetCreateSessionReport(bool bWasSuccessful);

	void TryJoinOnlineSession(const FOnlineSessionSearchResult& SearchResult);

	void OnGetJoinSessionReport(bool bWasSuccessful, FString TravleURL);

protected:

	void InitializeWidget();
};