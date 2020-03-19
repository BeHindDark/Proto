// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionCreator.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_SessionCreator : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UWG_SessionCreator(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UWG_SessionInfo* WG_SessionInfo;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* CreateSessionButton;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* ClearButton;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UButton* BacktoBrowserButton;

protected:
	TSubclassOf<UUserWidget> LoadingScreen_Class;

	UUserWidget* LoadingScreen;

private:
	class UWG_SessionBrowser* WG_SessionBrowser_Ref;

public:
	void SetUpperClassWidget(class UWG_SessionBrowser* SessionBrowser);

	void OnGetCreateSessionReport(bool bWasSuccessful);

protected:
	UFUNCTION()
	void OnCreateSessionClicked();

	UFUNCTION()
	void OnClearClicked();

	UFUNCTION()
	void OnBacktoBrowserClicked();
};
