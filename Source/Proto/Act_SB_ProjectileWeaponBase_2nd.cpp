// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_SB_ProjectileWeaponBase_2nd.h"
#include "WeaponControlSystem.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"

AAct_SB_ProjectileWeaponBase_2nd::AAct_SB_ProjectileWeaponBase_2nd()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	FireParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("FireEffectObject"));
	EffectFire = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("FireSystem"));
	
	Mesh->SetupAttachment(DefaultSceneRoot);

	Mesh->SetRelativeRotation(FRotator(90, -90, 0));

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_Fire(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (P_Fire.Succeeded())
	{
		FireParticle = P_Fire.Object;
	}

}

void AAct_SB_ProjectileWeaponBase_2nd::BeginPlay()
{

	Super::BeginPlay();
	
	UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	if (IsValid(Anim))
	{
		Anim->UpFireCheck.AddDynamic(this, &AAct_SB_ProjectileWeaponBase_2nd::UpFire);
		Anim->AnimationEnd.AddDynamic(this, &AAct_SB_ProjectileWeaponBase_2nd::AnimationEnd);
	}
	
}
void AAct_SB_ProjectileWeaponBase_2nd::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AAct_SB_ProjectileWeaponBase_2nd::GetArrowComponent_SB(UArrowComponent* Arrow1USceneComponent, USceneComponent* EX_Muzzle_Location)
{
	FirstArrow = Arrow1USceneComponent;
	ExMuzzle = EX_Muzzle_Location;

}


void AAct_SB_ProjectileWeaponBase_2nd::UpFire()
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
		FRotator Rotate = FirstArrow->GetComponentRotation();
		UWorld* World = GetWorld();

		if (World)
		{
			// AnimInstance의 IsAttacking 변수
			//this->IsAttacking = true;		// 무기 Actor의   IsAttacking 변수
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);
			if (FireParticle)
			{
				UGameplayStatics::SpawnEmitterAtLocation(World, FireParticle, Front)->AttachToComponent(FirstArrow, FAttachmentTransformRules::KeepRelativeTransform);

			}
		}
	}
}

void AAct_SB_ProjectileWeaponBase_2nd::AnimationEnd()
{

}