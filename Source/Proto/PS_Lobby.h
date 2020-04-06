// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PS_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API APS_Lobby : public APlayerState
{
	GENERATED_BODY()

public:
	APS_Lobby();


	FString m_PlayerName;
};
