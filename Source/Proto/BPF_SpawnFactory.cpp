// Fill out your copyright notice in the Description page of Project Settings.


#include "BPF_SpawnFactory.h"
#include "Ch_SpiderBase.h"
#include "Act_ProjectileWeaponBase.h"

bool UBPF_SpawnFactory::IsInitialized = false;
TArray<TSubclassOf<class AAct_WeaponBase>> UBPF_SpawnFactory::WeaponArray = {};
TArray<TSubclassOf<class ACh_SpiderBase>> UBPF_SpawnFactory::CharacterArray = {};

UBPF_SpawnFactory::UBPF_SpawnFactory(const FObjectInitializer& ObjectInitializer)
{

}

ACh_SpiderBase* UBPF_SpawnFactory::SpawnSpider( int32 CharacterClassIndex, FVector Location, FRotator Rotation, AActor* Owner)
{
	if (IsInitialized)
	{
		Initialize();
	}
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = Owner;
	auto Class = UBPF_SpawnFactory::CharacterArray[CharacterClassIndex];
	ACh_SpiderBase* NewActor =	Owner->GetWorld()->SpawnActor<ACh_SpiderBase>(Class, Location, Rotation,SpawnParam);
	UE_LOG(LogTemp, Warning, TEXT("SpawnSpiderExecute"));
	return NewActor;

}

AAct_WeaponBase* UBPF_SpawnFactory::SpawnWeapon(int32 WeaponClassIndex, FVector Location, FRotator Rotation, AActor* Owner)
{
	if (IsInitialized)
	{
		Initialize();
	}
	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = Owner;
	auto Class = UBPF_SpawnFactory::WeaponArray[WeaponClassIndex];
	auto NewActor = Owner->GetWorld()->SpawnActor<AAct_WeaponBase>(Class.Get(), Location, Rotation, SpawnParam);
	return NewActor;
}

void UBPF_SpawnFactory::Initialize()
{
	static ConstructorHelpers::FClassFinder<ACh_SpiderBase>Character_Spider(TEXT("/Game/Blueprints/TestSpider.TestSpider_C"));
	if (Character_Spider.Succeeded())
	{
		UE_LOG(LogTemp, Warning, TEXT("SpiderSourceExecute"));
		CharacterArray.Add(Character_Spider.Class);
	}
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