// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_CarrierFactory.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ACh_CarrierFactory::ACh_CarrierFactory()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FName TopSocket(TEXT("Mount_Top"));
	FName CockpitSocket(TEXT("Mount_Cockpit"));
	FName LShoulderSocket(TEXT("Mount_HalfShoulder_L"));
	FName RShoulderSocket(TEXT("Mount_HalfShoulder_R"));
	FName LWeaponSocket(TEXT("Mount_Weapon_L"));
	FName RWeaponSocket(TEXT("Mount_Weapon_R"));
	FName WeaponSocket(TEXT("Mount_Weapon"));

	FName TopComponentName(TEXT("ShoulderMesh"));
	
	const USkeletalMeshSocket* MainMeshSocket = GetMesh()->GetSocketByName(TopSocket);
	/*
	FName CockpitComponentName(TEXT("CockpitSocket"));
	FName LShoulderComponentName(TEXT("LShoulderMesh"));
	FName RShoulderComponentName(TEXT("RShoulderMesh"));
	FName LWeaponComponentName(TEXT("LWeaponMesh"));
	FName RWeaponComponentName(TEXT("RWeaponMesh"));
	FName WeaponComponentName(TEXT("WeaponMesh"));
	*/
	
	if (GetMesh()->DoesSocketExist(TopSocket)) {
		//애로우를 이용해서 간접적으로 붙이지말고 그냥 소켓에 직접적으로 붙이기
		//CockpitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CockpitArrow"));
		CockpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(TopComponentName);
		CockpitMesh->SetupAttachment(GetMesh(), TopSocket);
		//WCS.TargetWorldLocation = GetCameraAimLocation();
	}
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
