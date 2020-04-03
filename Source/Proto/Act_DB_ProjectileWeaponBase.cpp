// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_DB_ProjectileWeaponBase.h"
#include "Components/ArrowComponent.h"
#include "Anim_DB_Weapon_AnimInstance.h"
#include "Act_Bullet.h"



void AAct_DB_ProjectileWeaponBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void AAct_DB_ProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	//�������Ʈ�� �ִ��ν��Ͻ� �߰��ؾ���!

	/*UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	
	Anim->UpFireCheck.AddLambda([this]()->void {

		if (ProjectileClass)
		{

			UE_LOG(LogTemp, Warning, TEXT("if projectileClass is Execute"));
			FVector Front = FirstArrow->GetComponentLocation();
			FRotator Rotate = GetActorRotation();
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);

			}
		}//���ٸ��� �б׳��Լ��� �����ϱ� UFUCNTION ADDDYNAMIC UpFireCheck.AddDynamic(this, &AAct_DBkdjfladjf::�Լ��̸�);
		// 


		});
	Anim->DownFireCheck.AddLambda([this]()->void {

		if (ProjectileClass)
		{
			FVector Front = SecondArrow->GetForwardVector();
			FRotator Rotate = SecondArrow->GetRelativeRotation();
			UWorld* World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;
				 AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);

			}
		}


		});*/
}

AAct_DB_ProjectileWeaponBase::AAct_DB_ProjectileWeaponBase()
{
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));



}

void AAct_DB_ProjectileWeaponBase::GetArrowComponent(UArrowComponent* Arrow1, UArrowComponent* Arrow2)
{
	FirstArrow = Arrow1;
	SecondArrow = Arrow2;
}

void AAct_DB_ProjectileWeaponBase::UpFire()
{
	
	class UAnim_DB_Weapon_AnimInstance* Anim = Cast<UAnim_DB_Weapon_AnimInstance>(Mesh->GetAnimInstance());
	if (!IsValid(Anim)) {
		CHECK_LOG(!IsValid(Anim));
		return;
	}
	if (ProjectileClass)
	{

		UE_LOG(LogTemp, Warning, TEXT("if projectileClass is Execute"));
		FVector Front = FirstArrow->GetComponentLocation();
		FRotator Rotate = GetActorRotation();
		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);


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
			SpawnParams.Instigator = this->Instigator;
			AAct_Bullet* Bullet = World->SpawnActor<AAct_Bullet>(ProjectileClass, Front, Rotate, SpawnParams);


		}

	}

}
/*
void AAct_DB_ProjectileWeaponBase::SetAnimationDBWeapon(TSubclassOf<UAnimInstance> DB_Anim)//TsubclassOf�� Ŭ������ ������ �������ش�.
{
	UAnim_DB_Weapon_AnimInstance* anim = Cast<UAnim_DB_Weapon_AnimInstance>(DB_Anim);
	if (nullptr == anim)
	{
		//����ó��
		return;
	}

	if (IsValid(DB_Anim))
	{
		Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		Mesh->SetAnimInstanceClass(DB_Anim);
		//UAnimInstance::StaticClass()
	}

}

�ִϸ��̼��� �������Ʈ����
*/
//�����ش�.?Ŀ���� �ִ��ν��Ͻ� ���õ� �ִϸ��̼��� �츮������ C++�ִ� �ν��Ͻ�����? �ν����ؾߵȴ�. �ִ��ν��Ͻ����� ��Ƽ�����Լ�

