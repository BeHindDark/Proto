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
	//static���� �������ϳ� ��� �����Ծ�ȵ�

	UPROPERTY(BlueprintReadOnly)
	bool IsInitialized;

protected:


	static TArray<TSubclassOf<class AAct_WeaponBase>> WeaponArray ;

	static TArray<TSubclassOf<class ACh_SpiderBase>> CharacterArray ;
	 //���������� ����ƽ���� �ؾߵ�
	

};


