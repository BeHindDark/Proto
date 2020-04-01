// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionLobby.generated.h"

/**
 * 
 */

 /*
  * ALL = 모든 사용자와 대화
  * Team = 팀이랑 대화
  * Squad = 분대 대화
 */
UENUM(BlueprintType)
enum class ETeam : uint8
{
    eAll = 0 UMETA(DisplayName = "eAll"),
    eTeam UMETA(DisplayName = "eTeam"),
    eSquad UMETA(DisplayName = "eSquad")
};

UCLASS()
class PROTO_API UWG_SessionLobby : public UUserWidget
{
	GENERATED_BODY()

protected:

    virtual void NativeConstruct() override;
	
protected:
    UPROPERTY(meta = (BindWidget))
    class UButton* GameStartButton;

private:
    UFUNCTION()
    void OnGameStartClicked();
};
