// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/PlayerController.h"
#include "PC_Login.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API APC_Login : public APlayerController
{
	GENERATED_BODY()

	APC_Login();
public:
	class UWG_Join* WG_Join;

protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UWG_Login> WG_Login_Class;

	TSubclassOf<class UWG_Join> WG_Join_Class;

private:
	class UWG_Login* WG_Login_Ref;

public:
	void OnLogin(FText ID, FText PW);

	void OnJoin(FText ID, FText PW, FText PW2);

	void ShowJoinWG(int Zorder = 0);

	void InitializeWidget();
};
