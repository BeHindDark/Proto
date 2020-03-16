// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionLine.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_SessionLine : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	/** 마우스가 들어오면 배경을 밝게 하이라이트 시킵니다.*/
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

	/** 마우스가 나가면 다시 배경을 투명하게 합니다.*/
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/** 마우스로 위젯 클릭 시 세션 정보표시란에 해당 세션의 세부정보가 나타나게 한다.
	*	SessionBrowser 위젯의 관련함수를 호출한다.
	*/
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

	/** 마우스로 위젯을 더블 클릭하면 즉시 해당하는 세션에 참가한다.*/
	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditInstanceOnly, Category = UI)
	/** 이 SessionLine 위젯이 List 맨위에 나타나는 제목 열인지의 여부
	* 기본값은 false
	* true면 각 text는 적절한 목차를 표시하며
	* 마우스가 드나들어도 배경이 밝게 하이라이트 되지 않고
	* 마우스로 클릭 및 더블클릭이 불가능해 집니다.
	*/
	bool bIsTitle = false;

private:
	/** 이 SessionLine 위젯이 Browser, List에서 선택되었는지 여부
	*	선택되었을 시 값이 변경된다.
	*	전체 List에서 선택된 Line은 단 하나만 존재하도록 SessionBrowser에서 관리해줘야 한다.
	*/
	bool bIsSelected = false;

public:
	/**	위젯이 선택되거나 선택이 해제되었을 때 위젯의 색상을 변경하고 bIsSelected변수를 변경합니다.
	*	bIsSelected : 이 SessionLine 위젯이 SessionBrowser, SessionList에서 선택된 상태인지 여부
	*	@param	Selected	새로운 bIsSelected 값
	*/
	void SetSelect(bool Selected);

	/**	현재 bIsSelected변수를 받아옵니다.
	*	bIsSelected : 이 SessionLine 위젯이 SessionBrowser, SessionList에서 선택된 상태인지 여부
	*/
	bool GetIsSelected();


protected:
	/** 이 SessionLine과 연동되는 세션의 정보
	*/
	FOnlineSessionSearchResult SessionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Color")
	FLinearColor NormalBackgroundColor = FLinearColor(0.0f, 0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Color")
	FLinearColor NormalContentColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Color")
	FLinearColor HoveredBackgroundColor = FLinearColor(0.8f, 1.0f, 1.0f, 1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Color")
	FLinearColor SelectedBackgroundColor = FLinearColor(0.8f, 1.0f, 1.0f, 0.8f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Color")
	FLinearColor SelectedContentColor = FLinearColor(0.6f, 0.6f, 0.6f, 1.0f);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI|Color")
	FLinearColor TitleBoxColor = FLinearColor(0.08f,0.35f,0.35f,1.0f);

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category = "UI|Color")
	FLinearColor NormalBoxColor = FLinearColor(0.04f,0.18f,0.18f,0.8f);

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category = "UI|Color")
	FLinearColor CurrentBoxColor = FLinearColor(0.04f,0.18f,0.18f,0.8f);


	/** 상위 위젯인 SessionBrowser에 대한 레퍼런스
	*/
	class UWG_SessionBrowser* SessionBrowser_Ref;

	/** 상위 위젯인 SessionList에 대한 레퍼런스
	*/
	class UWG_SessionList* SessionList_Ref;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UBorder* BackgroundBorder;
	
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* SessionNameText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* MapText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* ModeText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* CurrentPlayerText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PlayerSeatText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* MaxPlayerText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* PingText;

public:
	/**
	*	새로운 세션 정보를 받아 각 Text를 업데이트 합니다.
	*	@param	NewSession	새로운세션검색결과 FOnlineSessionSearchResult
	*	@return	bool		주어진 세션정보가 valid하여 업데이트를 성공했으면 true invalid하면 false
	*/
	bool UpdateSessionData(FOnlineSessionSearchResult NewSession);

	void SetUpperClass(class UWG_SessionList* SessionList,class UWG_SessionBrowser* SessionBrowser);
};