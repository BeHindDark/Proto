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
	 //static���� �������ϳ� ��� �����Ծ�ȵ�
	 bool IsInitialized;

protected:


	 TArray<TSubclassOf<class AAct_WeaponBase>> WeaponArray;

	 TArray<TSubclassOf<class ACh_SpiderBase>> CharacterArray;
	 //���������� ����ƽ���� �ؾߵ�

};
