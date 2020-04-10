// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "BPF_SpawnFactory.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UBPF_SpawnFactory : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UBPF_SpawnFactory(const FObjectInitializer& ObjectInitializer);


	 class ACh_SpiderBase* SpawnSpider(int32 CharacterClassIndex, FVector Location, FRotator Rotation, AActor* Owner);
	 class AAct_WeaponBase* SpawnWeapon(int32 WeaponClassIndex, FVector Location, FRotator Rotation, AActor* Owner);
	 //static으로 만들어야하나 계속 에러먹어서안됨
	 bool IsInitialized;

protected:


	 TArray<TSubclassOf<class AAct_WeaponBase>> WeaponArray;

	 TArray<TSubclassOf<class ACh_SpiderBase>> CharacterArray;
	 //마찬가지로 스태틱으로 해야됨

};
