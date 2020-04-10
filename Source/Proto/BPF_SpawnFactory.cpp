// Fill out your copyright notice in the Description page of Project Settings.


#include "BPF_SpawnFactory.h"
#include "Ch_SpiderBase.h"
#include "Act_ProjectileWeaponBase.h"

TArray<TSubclassOf<class AAct_WeaponBase>>UBPF_SpawnFactory::WeaponArray = {};
TArray<TSubclassOf<class ACh_SpiderBase>> UBPF_SpawnFactory::CharacterArray = {};

UBPF_SpawnFactory::UBPF_SpawnFactory(const FObjectInitializer& ObjectInitializer)
{

	UBPF_SpawnFactory::WeaponArray.Empty();
	UBPF_SpawnFactory::CharacterArray.Empty();
	CharacterArray.SetNum(1);
	static ConstructorHelpers::FClassFinder<ACh_SpiderBase>Character_Spider(TEXT("/Game/Blueprints/TestSpider.TestSpider_C"));
	if (Character_Spider.Succeeded())
	{
		CharacterArray.Add(Character_Spider.Class);
	}
	WeaponArray.SetNum(2);
	static ConstructorHelpers::FClassFinder<AAct_WeaponBase>Actor_Weapon(TEXT("/Game/Blueprints/BP_Act_DB_ProjectileWeaponBase.BP_Act_DB_ProjectileWeaponBase_C"));
	if (Actor_Weapon.Succeeded())
	{
		WeaponArray.Add(Actor_Weapon.Class);
	}

	static ConstructorHelpers::FClassFinder<AAct_WeaponBase>Actor_Weapon2(TEXT("/Game/Blueprints/BP_Act_SB_ProjectileWeaponBase.BP_Act_SB_ProjectileWeaponBase_C"));
	if (Actor_Weapon2.Succeeded())
	{
		WeaponArray.Add(Actor_Weapon2.Class);
	}

	IsInitialized = true;
}

ACh_SpiderBase* UBPF_SpawnFactory::SpawnSpider(UWorld* World, int32 CharacterClassIndex, FVector Location, FRotator Rotation, AActor* Owner)
{
	auto Class = UBPF_SpawnFactory::CharacterArray[CharacterClassIndex];
	auto NewActor = World->SpawnActor<ACh_SpiderBase>(Class.Get(), Location, Rotation,FActorSpawnParameters());
	return NewActor;
}

AAct_WeaponBase* UBPF_SpawnFactory::SpawnWeapon(UWorld* World,int32 WeaponClassIndex, FVector Location, FRotator Rotation, AActor* Owner)
{
	auto Class = UBPF_SpawnFactory::WeaponArray[WeaponClassIndex];
	auto NewActor =World->SpawnActor<AAct_WeaponBase>(Class, Location, Rotation);
	return NewActor;
}

