// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_SBProjectileWeaponBase.h"
#include "WeaponControlSystem.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
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


	////총알Actor 생성
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

void AAct_SBProjectileWeaponBase::GetArrowComponent_SB(UArrowComponent* Arrow1)
{
	FirstArrow = Arrow1;
}

void AAct_SBProjectileWeaponBase::UpFire()
{
	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
		
	if (!IsValid(Anim)) {
		CHECK_LOG(!IsValid(Anim));
		return;
	}

	if (ProjectileClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("If projectileClass is Execute."));
		FVector Front = FirstArrow->GetComponentLocation();
		FRotator Rotate = GetActorRotation();
		UWorld* World = GetWorld();

		if (World)
		{
			Anim->IsAttacking = true;		// AnimInstance의 IsAttacking 변수
			this->IsAttacking = true;		// 무기 Actor의   IsAttacking 변수
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);
		}
	}
}

// Called every frame
void AAct_SBProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

