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

	FName ShoulderSocket(TEXT("Mount_Top"));
	FName CockpitSocket(TEXT("Mount_Cockpit"));
	FName LShoulderSocket(TEXT("Mount_HalfShoulder_L"));
	FName RShoulderSocket(TEXT("Mount_HalfShoulder_R"));
	FName LWeaponSocket(TEXT("Mount_Weapon_L"));
	FName RWeaponSocket(TEXT("Mount_Weapon_R"));

	FName ShoulderComponentName(TEXT("ShoulderMesh"));
	FName CockpitComponentName(TEXT("CockpitSocket"));
	FName LShoulderComponentName(TEXT("LShoulderMesh"));
	FName RShoulderComponentName(TEXT("RShoulderMesh"));
	FName LWeaponComponentName(TEXT("LWeaponMesh"));
	FName RWeaponComponentName(TEXT("RWeaponMesh"));

	if (GetMesh()->DoesSocketExist(ShoulderSocket)) {
		//�ַο츦 �̿��ؼ� ���������� ���������� �׳� ���Ͽ� ���������� ���̱�
		//CockpitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CockpitArrow"));
		CockpitMesh = CreateDefaultSubobject<UStaticMeshComponent>(ShoulderComponentName);
		CockpitMesh->SetupAttachment(GetMesh(), ShoulderSocket);
	}

	//��� ��üȭ�غôµ� ���������� ��....����?
	if (CockpitMesh) {
		AttachMesh(CockpitSocket, ShoulderMesh, CockpitMesh, CockpitComponentName);
	}
	//������� ��밨���� ����
	if (LShoulderMesh) {
		AttachMesh(LShoulderSocket, CockpitMesh, LShoulderMesh, LShoulderComponentName);
		AttachMesh(RShoulderSocket, CockpitMesh, RShoulderMesh, RShoulderComponentName);
		AttachMesh(LWeaponSocket, LShoulderMesh, LWeaponMesh, LWeaponComponentName);
		AttachMesh(RWeaponSocket, RShoulderMesh, RWeaponMesh, RWeaponComponentName);
	}
	else if (LWeaponMesh) {
		AttachMesh(LWeaponSocket, CockpitMesh, LWeaponMesh, LWeaponComponentName);
		AttachMesh(RWeaponSocket, CockpitMesh, RWeaponMesh, RWeaponComponentName);
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

void ACh_CarrierFactory::AttachMesh(FName SocketName, UStaticMeshComponent* MotherMeshName, UStaticMeshComponent* AttachMeshName, FName ComponentName) {
	if (MotherMeshName->DoesSocketExist(SocketName)) {
		AttachMeshName = CreateDefaultSubobject<UStaticMeshComponent>(ComponentName);
		AttachMeshName->SetupAttachment(MotherMeshName, SocketName);
	}
}
