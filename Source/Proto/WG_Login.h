// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_Login.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Login : public UUserWidget
{
	GENERATED_BODY()


protected:

	UFUNCTION(BlurprintCallable)
	void IDCommitted();
};
