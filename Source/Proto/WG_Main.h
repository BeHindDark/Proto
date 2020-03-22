// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_Main.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Main : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* SessionBrowserButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* AmoryButton;

protected:
    UFUNCTION()
    void SessionBrowserButtonClicked();

    UFUNCTION()
    void AmoryButtonClicked();
};
