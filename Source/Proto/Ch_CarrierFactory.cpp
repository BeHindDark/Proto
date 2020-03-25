// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_CarrierFactory.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
ACh_CarrierFactory::ACh_CarrierFactory()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CockpitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CockpitArrow"));
	LeftWeaponArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LWeaponArrow"));
	RightWeaponArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RWeaponArrow"));
	LeftShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LShoulderArrow"));
	RightShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RShoulderArrow"));
	ArrowArrayIndex = { CockpitArrow, LeftWeaponArrow, RightWeaponArrow, LeftShoulderArrow, RightShoulderArrow };

	Root = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Root"));
	LegMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LegMesh"));
	CockpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CockpitMesh"));
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	RootComponent = Root;

	LegMesh->SetupAttachment(Root);
	CockpitArrow->SetupAttachment(LegMesh);

	//���� �̰� ���� �ȵ�. ���۸���
	//CockpitMesh->SetupAttachment(LegMesh);
	LeftWeaponArrow->SetupAttachment(CockpitMesh);
	RightWeaponArrow->SetupAttachment(CockpitMesh);

	//���� �̰� ���� �ȵ�. ���۸���
	//CockpitMesh->SetupAttachment(LegMesh);
	WeaponMesh->SetupAttachment(CockpitMesh);

}

// Called when the game starts or when spawned
void ACh_CarrierFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACh_CarrierFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACh_CarrierFactory::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

