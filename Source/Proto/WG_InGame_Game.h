// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_InGame_Game.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_InGame_Game : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* UserIDText;

};
