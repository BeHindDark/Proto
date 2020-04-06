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

    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    // class UWG_Chat* UMG_Chat;

    //UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    //class UWG_LobbyTeam* UMG_LobbyTeam;

    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UButton* GameStartButton;

protected:
    // �� ���ε�
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
    
    // ä�� ���ε� 
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