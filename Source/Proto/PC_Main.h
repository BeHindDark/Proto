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

	UPROPERTY()
	TSubclassOf<class UWG_Main> WG_Main_Class;

	UPROPERTY()
	class UWG_Main* WG_Main;

};
