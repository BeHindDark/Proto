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

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry,const FPointerEvent& InMouseEvent) override;

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = UI)
	bool bIsTitle = false;

	FOnlineSessionSearchResult SessionData;

protected:

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

};