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
	//블루프린트에 애님인스턴스 추가해야함!

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
		}//람다말고 ㅠ그냥함수로 변경하기 UFUCNTION ADDDYNAMIC UpFireCheck.AddDynamic(this, &AAct_DBkdjfladjf::함수이름);
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
void AAct_DB_ProjectileWeaponBase::SetAnimationDBWeapon(TSubclassOf<UAnimInstance> DB_Anim)//TsubclassOf는 클래스의 범위를 제한해준다.
{
	UAnim_DB_Weapon_AnimInstance* anim = Cast<UAnim_DB_Weapon_AnimInstance>(DB_Anim);
	if (nullptr == anim)
	{
		//에러처리
		return;
	}

	if (IsValid(DB_Anim))
	{
		Mesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
		Mesh->SetAnimInstanceClass(DB_Anim);
		//UAnimInstance::StaticClass()
	}

}

애니메이션은 블루프린트에서
*/
//묶어준다.?커스텀 애님인스턴스 세팅된 애니메이션이 우리가만든 C++애님 인스턴스인지? 인식을해야된다. 애님인스턴스에서 노티파이함수

