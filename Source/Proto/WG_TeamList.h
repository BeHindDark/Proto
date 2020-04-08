// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_TeamList.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_TeamList : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual void NativeConstruct() override;
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UVerticalBox* PlayerListSpace;
};
