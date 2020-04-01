// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_LobbyTeam.generated.h"

/**
 *
 */
UCLASS()
class PROTO_API UWG_LobbyTeam : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
		class UButton* RedButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* BlueButton;

public:

	void RedClicked();
	void BlueClicked();
};
