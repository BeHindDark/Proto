// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_SBProjectileWeaponBase.h"
#include "WeaponControlSystem.h"
#include "Components/ArrowComponent.h"
#include "Act_Bullet.h"

// Sets default values
AAct_SBProjectileWeaponBase::AAct_SBProjectileWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	IsAttacking = false;
}

// Called when the game starts or when spawned
void AAct_SBProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();


	////ÃÑ¾ËActor »ý¼º
	//FName MuzzleSocket(TEXT("Muzzle"));
	//auto CurBullet = GetWorld()->SpawnActor<AAct_Bullet>(FVector::ZeroVector, FRotator::ZeroRotator);
	//if (nullptr != CurBullet)
	//{
	//	CurBullet->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, MuzzleSocket);
	//}
	
}

void AAct_SBProjectileWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	auto AnimInstance = Cast<UAnimInstance>(Mesh->GetAnimInstance());
	if (nullptr != AnimInstance) {

	}
	else {
		//AnimInstance->UpFireCheck.AddDynamic(this, && AAct_SBProjectileWeaponBase::);
	}

}

// Called every frame
void AAct_SBProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

