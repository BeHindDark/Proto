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

	//�޸� : DamageCauser�� Act_BaseWeapon���� Cast�� ��, �̸��� �޾ƿ� �� �ְ�, ���� �̸� ������ ������ �Ѵ�.

	if(!IsValid(EventInstigator))
	{
		InstigatorType = EDamageInstigatorType::DamageInstigator_Environmental;

		//DamageEvent�� �м��Ͽ� ������ InstigatorName�� �ο��� �� �ִ�.
	}
	else
	{
		//���⼭ ���� PlayerController�� �ʿ��ϴ�. �÷��̾� ��Ʈ�ѷ� �θ�Ŭ������ Team ������ �����, DamageControlSystem���� Team ������ ����� ���� ���ؾ� �Ѵ�.
		//���� PlayerController�� ���� InstigatorName�� �޾ƾ� �Ѵ�.
		//�ϴ��� �ӽ÷� ������ ����
		InstigatorType = EDamageInstigatorType::DamageInstigator_Enemy;
		InstigatorName = EventInstigator->GetName();
	}

	//���⼭ ����, �����, ������ ���� � ���� ���� ������ Damage ���� �����Ѵ�.
	float DamageApply = Damage;

	//������ HP�� ���մϴ�.
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