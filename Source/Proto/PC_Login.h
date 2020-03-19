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
protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UWG_Login> WG_Login_Class;

private:
	class UWG_Login* WG_Login_Ref;

public:
	void OnLogin(FText ID, FText PW);
};
