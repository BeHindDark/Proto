// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionBrowser.generated.h"


UCLASS()
/**	SessionBrowser는 세션을 검색하고
 *	그 검색결과를 SessionList에 표시하며,
 *	특정세션에 대한 참가요청을 PC_Main에 요청합니다.
 */
class PROTO_API UWG_SessionBrowser : public UUserWidget
{
	GENERATED_BODY()

public:
	UWG_SessionBrowser(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWG_SessionList* WG_SessionList;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UWG_SessionInfo* WG_SessionInfo;
	
	/**	WG_SessionInfo에 표시된 세션으로 참가를 요청합니다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* JoinButton;

	/**	WG_SessionList를 비우고 다시 세션을 검색합니다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* RefreshAllButton;

	/**	현재 WG_SessionList에 있는 SessionLine들의 정보를 업데이트 합니다.
	*	만약을 위해 남겨둔 예비 변수입니다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* QuickRefreshButton;

	/**	Visibility를 Hidden으로 두고 PC_Main을 통해 WG_SessionCreator를 엽니다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* SessionCreatorButton;

	/**	스스로를 제거하고, PC_Main을 통해 WG_Main을 엽니다.
	*/
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* BacktoMainButton;

protected:
	TSubclassOf<class UWG_SessionLine> WG_SessionLine_Class;

private:

	bool functionbrake = false;

	FTimerDelegate BrakeTimer;

	FTimerHandle BrakeTimerHandle;

public:

	void OnGetFindSessionReport(bool bWasSuccessful);

protected:
	UFUNCTION()
	void OnJoinButtonClicked();

	UFUNCTION()
	void OnRefreshAllButtonClicked();

	UFUNCTION()
	/**	QuickRefresh 버튼이 눌렸을 때 SessionLine들을 업데이트 하는 함수입니다.
	*	현재 언리얼 엔진이 FindSessionById를 틀만만들어두고 채워두지 않은 상황이라
	*	oculus 세션일 떄 외에는 사용이 불가능합니다.
	*	후일 언리얼이 기능을 채워넣었을 떄를 대비하여 일단 남겨둡니다.
	*/
	void OnQuickRefreshButtonClicked();

	UFUNCTION()
	void OnSessionCreatorButtonClicked();

	UFUNCTION()
	void OnBacktoMainButtonClicked();

	void CallFindSession();
};
