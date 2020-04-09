// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Oj_Build_Act.generated.h"

/**
 * 
 */
UCLASS(config=Game)
class PROTO_API UOj_Build_Act : public UObject
{
	GENERATED_BODY()
	
public:
	UOj_Build_Act();

	UPROPERTY(config,BlueprintReadOnly,Category="Asset")
		TArray<FSoftObjectPath> CharacterAssets;
};
