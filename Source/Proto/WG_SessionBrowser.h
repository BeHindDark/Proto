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

protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class WG_SessionList* WG_SessionList;
	
};
