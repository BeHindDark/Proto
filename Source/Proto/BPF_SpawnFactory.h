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

	UFUNCTION(BlueprintCallable)
	static class ACh_SpiderBase* SpawnSpider(UWorld* World, int32 CharacterClassIndex, FVector Location, FRotator Rotation, AActor* Owner);

	UFUNCTION(BlueprintCallable)
	static class AAct_WeaponBase* SpawnWeapon(UWorld* World, int32 WeaponClassIndex, FVector Location, FRotator Rotation, AActor* Owner);
	//static으로 만들어야하나 계속 에러먹어서안됨

	UPROPERTY(BlueprintReadOnly)
	bool IsInitialized;

protected:


	static TArray<TSubclassOf<class AAct_WeaponBase>> WeaponArray ;

	static TArray<TSubclassOf<class ACh_SpiderBase>> CharacterArray ;
	 //마찬가지로 스태틱으로 해야됨
	

};


