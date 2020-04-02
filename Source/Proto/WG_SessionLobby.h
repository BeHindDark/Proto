// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionLobby.generated.h"

/**
 * 
 */

 /*
  * ALL = ��� ����ڿ� ��ȭ
  * Team = ���̶� ��ȭ
  * Squad = �д� ��ȭ
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
	
public:
    UPROPERTY(meta = (BindWidget))
    class UWG_Chat* WG_Chat;

    UPROPERTY(meta = (BindWidget))
    class UWG_LobbyTeam* WG_LobbyTeam;

    UPROPERTY(meta = (BindWidget))
    class UButton* GameStartButton;

private:
    UFUNCTION()
    void OnGameStartClicked();
};
