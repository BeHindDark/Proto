// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_DB_ProjectileWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"



void AAct_DB_ProjectileWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
		 
}
AAct_DB_ProjectileWeaponBase::AAct_DB_ProjectileWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->AddRelativeRotation(FRotator(90, -90, 0));
	FireParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("EFFECT"));
	FireParticle->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UParticleSystem>P_Fire(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));
	if (P_Fire.Succeeded())
	{
		FireParticle->SetTemplate(P_Fire.Object);
		FireParticle->bAutoActivate = false;
	}

}


void AAct_DB_ProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();

	//블루프린트에 애님인스턴스 추가해야함!

	UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	Anim->UpFireCheck.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::UpFire);
	Anim->DownFireCheck.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::DownFire);
	
	/** 이펙트를 지정해줍니다.
	*/
	
	
}


void AAct_DB_ProjectileWeaponBase::GetArrowComponent(UArrowComponent* Arrow1, UArrowComponent* Arrow2)
{
	FirstArrow = Arrow1;
	SecondArrow = Arrow2;
}

void AAct_DB_ProjectileWeaponBase::UpFire()
{

	UE_LOG(LogTemp, Warning, TEXT("UpFire is Execute"));
	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	if (!IsValid(Anim)) {

		UE_LOG(LogTemp, Warning, TEXT("if Anim is Execute"));
		CHECK_LOG(!IsValid(Anim));
		return;
	}
	if (ProjectileClass)
	{

		//GameStatic->SpawnEmitterAttached(FireParticle, Mesh, FName("Barrel_End_1"));

		//Barrel_End_1
		UE_LOG(LogTemp, Warning, TEXT("if projectileClass is Execute"));
		FVector Front = FirstArrow->GetComponentLocation();
		FRotator Rotate = GetActorRotation();
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);
			FireParticle->Activate(true);
			FireParticle->OnSystemFinished.AddDynamic(this, &AAct_DB_ProjectileWeaponBase::OnEffectFinished);

		}

	}

}
void AAct_DB_ProjectileWeaponBase::DownFire()
{
	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());

	if (!IsValid(Anim)) {
		CHECK_LOG(!IsValid(Anim));
		return;
	}
	if (ProjectileClass)
	{

		UE_LOG(LogTemp, Warning, TEXT("if projectileClass is Execute"));
		FVector Front = SecondArrow->GetComponentLocation();
		FRotator Rotate = GetActorRotation();
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			//SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);

		}

	}

}
void AAct_DB_ProjectileWeaponBase::OnEffectFinished(class UParticleSystemComponent* PSystem)
{
	FireParticle->Activate(false);
}
