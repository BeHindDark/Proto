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

	FName CockpitSocket(TEXT("Mount_Top"));
	if (GetMesh()->DoesSocketExist(CockpitSocket)) {
		//애로우를 이용해서 간접적으로 붙이지말고 그냥 소켓에 직접적으로 붙이기
		//CockpitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CockpitArrow"));
		CockpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CockpitMesh"));
		CockpitMesh->SetupAttachment(GetMesh(), CockpitSocket);

		FName LWeaponSocket(TEXT("Mount_Weapon_L"));
		if (CockpitMesh->DoesSocketExist(LWeaponSocket)) {
			//LeftWeaponArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LWeaponArrow"));
			WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LWeaponMesh"));
			WeaponMesh->SetupAttachment(CockpitMesh, LWeaponSocket);
		}

		FName RWeaponSocket(TEXT("Mount_Weapon_R"));
		if (CockpitMesh->DoesSocketExist(RWeaponSocket)) {
			WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RWeaponMesh"));
			WeaponMesh->SetupAttachment(CockpitMesh, RWeaponSocket);
		}
	}
	/*FName LShoulderSocket(TEXT("Mount_HalfShoulder_L"));
	if (CockpitMesh->DoesSocketExist(LShoulderSocket)) {

	}
	FName RShoulderSocket(TEXT("Mount_HalfShoulder_R"));
	if (CockpitMesh->DoesSocketExist(RShoulderSocket)) {

	}*/

	//RightWeaponArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RWeaponArrow"));
	//LeftShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("LShoulderArrow"));
	//RightShoulderArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("RShoulderArrow"));
	//ArrowArrayIndex = { CockpitArrow, LeftWeaponArrow, RightWeaponArrow, LeftShoulderArrow, RightShoulderArrow };

	

	//CockpitArrow->SetupAttachment(GetMesh());

	//LeftWeaponArrow->SetupAttachment(CockpitMesh);
	//RightWeaponArrow->SetupAttachment(CockpitMesh);
	//LeftShoulderArrow->SetupAttachment(CockpitMesh);
	//RightShoulderArrow->SetupAttachment(CockpitMesh);

	//CockpitMesh->SetupAttachment(GetMesh());
	//WeaponMesh->SetupAttachment(CockpitMesh);

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

