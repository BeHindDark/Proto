// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageControlSystem.h"

// Sets default values for this component's properties
UDamageControlSystem::UDamageControlSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDamageControlSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UDamageControlSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

float UDamageControlSystem::GetHPRatio()
{
	if(CurrentMaxHP<=KINDA_SMALL_NUMBER)
	{
		return 0.0f;
	}
	else
	{
		return CurrentHP/CurrentMaxHP;
	}
}

float UDamageControlSystem::GetCurrentMaxHP()
{
	return CurrentMaxHP;
}

float UDamageControlSystem::GetCurrentHP()
{
	return CurrentHP;
}

void UDamageControlSystem::InitializeSystem(float MaxHP)
{
	InitialMaxHP = MaxHP;
	CurrentMaxHP = MaxHP;
	CurrentHP = MaxHP;
}

void UDamageControlSystem::StartSystem()
{
	if(GetOwnerRole()<ROLE_Authority)
	{
		Server_RequestDataSync();
	}
}

float UDamageControlSystem::OnTakeDamage(float Damage,FDamageEvent const & DamageEvent, class AController * EventInstigator, class AActor * DamageCauser)
{
	if(GetOwnerRole()<ROLE_Authority)
	{
		return 0.0f;
	}
	EDamageInstigatorType InstigatorType = EDamageInstigatorType::DamageInstigator_None;
	FString InstigatorName = TEXT("None");

	//메모 : DamageCauser를 Act_BaseWeapon으로 Cast한 후, 이름을 받아올 수 있게, 무기 이름 변수를 만들어야 한다.

	if(!IsValid(EventInstigator))
	{
		InstigatorType = EDamageInstigatorType::DamageInstigator_Environmental;

		//DamageEvent를 분석하여 적절한 InstigatorName을 부여할 수 있다.
	}
	else
	{
		//여기서 공용 PlayerController가 필요하다. 플레이어 컨트롤러 부모클래스에 Team 변수를 만들고, DamageControlSystem에도 Team 변수를 만들어 둘을 비교해야 한다.
		//또한 PlayerController를 통해 InstigatorName을 받아야 한다.
		//일단은 임시로 적으로 가정
		InstigatorType = EDamageInstigatorType::DamageInstigator_Enemy;
		InstigatorName = EventInstigator->GetName();
	}

	//여기서 버프, 디버프, 데미지 종류 등에 따라 실제 적용할 Damage 양을 결정한다.
	float DamageApply = Damage;

	//적용할 HP를 정합니다.
	float NewCurrentHP = CurrentHP - Damage;
	if(NewCurrentHP<=0.0f)
	{
		Multicast_AfterDamageApplied(Damage, 0.0f, CurrentMaxHP, DamageCauser, InstigatorType, InstigatorName);
		OnHPIsZero.Broadcast(EventInstigator, DamageCauser, GetOwner());
	}
	else
	{
		Multicast_AfterDamageApplied(Damage, NewCurrentHP, CurrentMaxHP, DamageCauser, InstigatorType, InstigatorName);
	}
	
	return DamageApply;
}

void UDamageControlSystem::Multicast_AfterDamageApplied_Implementation(float Damage,float NewCurrentHP,float NewCurrentMaxHP, AActor * DamageCauser, EDamageInstigatorType DamageInstigatorType, const FString & DamageInstigatorName)
{
	CurrentHP = NewCurrentHP;
	CurrentMaxHP = NewCurrentMaxHP;
	OnHPIsChanged.Broadcast();
}

void UDamageControlSystem::Server_RequestDataSync_Implementation()
{
	Multicast_DataSync(CurrentHP, CurrentMaxHP);
}

void UDamageControlSystem::Multicast_DataSync_Implementation(float ServerHP,float ServerMaxHP)
{
	if(GetOwnerRole()<ROLE_Authority)
	{
		CurrentHP = ServerHP;
		CurrentMaxHP = ServerMaxHP;
		OnHPIsChanged.Broadcast();
	}
}