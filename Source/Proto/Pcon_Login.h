// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/PlayerController.h"
#include "Pcon_Login.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API APcon_Login : public APlayerController
{
	GENERATED_BODY()
	
	APcon_Login();
protected:
	virtual void BeginPlay() override;

	TSubclassOf<class UWG_Login> WG_Login_Class;

private:
	class UWG_Login* WG_Login_Ref;
};
