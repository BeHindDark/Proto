// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_TeamName.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_TeamName : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* PlayerName;

	FString Name;

	void SetUserName(FString _UserName);

};
