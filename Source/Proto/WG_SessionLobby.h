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
	
public:

    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    // class UWG_Chat* UMG_Chat;

    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //class UWG_LobbyTeam* UMG_LobbyTeam;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* GameStartButton;

protected:
    // 팀 바인딩
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UVerticalBox* RedPlayerList;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UVerticalBox* BluePlayerList;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UVerticalBox* UnchosenPlayers;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* RedButton;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* BlueButton;
    
    // 채팅 바인딩 
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UEditableTextBox* ChatInput;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UScrollBox* ChatBox;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* ChatMode;

private:
    UFUNCTION()
    void OnGameStartClicked();


    UFUNCTION()
    void OnInputChatCommit(const FText& Input, ETextCommit::Type InCommitType);
public:

    void UpdateChatBox(const FString& message);
    
    void RedClicked();
    void BlueClicked();
    
    void UpdateRedList(const FString& Name);
    void UpdateBlueList(const FString& Name);
    void UnchosenPlayersList(const FString& Name);
};