// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionBrowser.generated.h"

/**
 * 
 */
UCLASS()
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

protected:
	TSubclassOf<class UWG_SessionLine> WG_SessionLine_Class;

	TSubclassOf<class UWG_SessionCreator> WG_SessionCreator_Class;

};
