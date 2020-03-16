// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionList.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_SessionList : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UScrollBox* SessionScrollBox;

protected:
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,meta=(BindWidget))
	class UWG_SessionLine* TitleLine;

public:
	void ClearSelection();
};
