// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "WeaponControlSystem.h"
#include "Act_Bullet.h"

// Sets default values
AAct_WeaponBase::AAct_WeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//리플리케이션 설정
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	RootComponent = DefaultSceneRoot;
		
}

void AAct_WeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	//변수 리플리케이션을 할 때 항상 삽입해야 하는 함수입니다.
	//선언은 없이 정의부분만 넣어주면 됩니다.
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAct_WeaponBase,TargetLocation);
}

// Called when the game starts or when spawned
void AAct_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters spawnparam;
	
	//GetWorld()->SpawnActor<AAct_Bullet>()
	//GetWorld()->SpawnActorDiffed<>
	//https://docs.unrealengine.com/en-US/API/Runtime/Engine/Engine/UWorld/SpawnActorDeferred/index.html
}

float AAct_WeaponBase::TakeDamage(float DamageAmount,FDamageEvent const & DamageEvent,AController * EventInstigator,AActor * DamageCauser)
{
	OnWeaponTakeDamage.Broadcast(DamageAmount,DamageEvent,EventInstigator,DamageCauser);
	return DamageAmount;
}

// Called every frame
void AAct_WeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	/*
	if(GetLocalRole()>=ROLE_Authority)
	{
		TurnTowardDirectAim(DeltaTime);
	}
	*/

}


void AAct_WeaponBase::ServerOnFireOrder()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	//이제 하위 무기에 맞게 오버라이드 하여 사용하면 됩니다.
	UE_LOG(Proto,Warning,TEXT("%s / %s : FireOrder"),*LINE_INFO,*GetNameSafe(this));
}

void AAct_WeaponBase::ServerOnCeaseFireOrder()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	//이제 하위 무기에 맞게 오버라이드 하여 사용하면 됩니다.
	UE_LOG(Proto,Warning,TEXT("%s / %s : CeaseFireOrder"),*LINE_INFO,*GetNameSafe(this));
}

void AAct_WeaponBase::ConnectWeaponControlSystem_Implementation(UWeaponControlSystem * NewWeaponControlSystem,int NewWeaponIndex)
{
	WeaponControlSystem_Ref = NewWeaponControlSystem;
	if(!IsValid(WeaponControlSystem_Ref))
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : fuckfuckfuck"),*LINE_INFO,*GetNameSafe(this));
	}

	SocketArrow_Ref = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].SocketArrow_Ref;

	WeaponIndex = NewWeaponIndex;

	PitchRotationLimit = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].PitchRotationLimit;

	YawRotationLimit = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].PitchRotationLimit;
}

void AAct_WeaponBase::SetSingleMuzzleArrow(UArrowComponent * MuzzleArrow)
{
	SingleMuzzleArrow = MuzzleArrow;
}

UWeaponControlSystem * AAct_WeaponBase::GetWeaponControlSystem()
{
	return WeaponControlSystem_Ref;
}

void AAct_WeaponBase::TurnTowardDirectAim(float DeltaTime)
{
	if(!IsValid(WeaponControlSystem_Ref))
	{
		return;
	}

	if(!SocketArrow_Ref.IsValid())
	{
		return;
	}

	TargetLocation = WeaponControlSystem_Ref->TargetWorldLocation;

	FTransform SocketTransform = SocketArrow_Ref.Get()->GetComponentTransform();

	//소켓을 기준으로 했을 때 타켓의 위치를 구한 뒤
	//ToOrientationRotator로 해당 위치를 바라보도록 하는 rotation을 구한다. 이때 roll=0으로 고정된다.
	FRotator RelativeTargetDirection = FTransform(FRotator::ZeroRotator,TargetLocation).GetRelativeTransform(SocketTransform).GetLocation().ToOrientationRotator();

	RelativeTargetDirection.Roll = 0.0f;

	//소켓에 대한 포탑의 현재 rotation
	FRotator CurRelativeRotation = GetActorTransform().GetRelativeTransform(SocketTransform).Rotator();
	

	//목표지점을 바라보기 위해 포탑이 가져야하는 상대회전값
	//ClampAngle을 통해 최대 부앙각을 제한시킨다.

	FRotator TargetRelativeRotation = FRotator(FMath::ClampAngle(RelativeTargetDirection.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]),
											   FMath::ClampAngle(RelativeTargetDirection.Yaw,YawRotationLimit[0],YawRotationLimit[1]),
											   0.0f);
	   

	//타겟을 바라보게 하기 위해 회전시켜야 하는 총각도
	FRotator RotationDiff = FRotator::ZeroRotator;
	//덜어내기 용 더미 각도
	FRotator Dummyrot = FRotator::ZeroRotator;

	//두 각도의 차이를 구한 뒤 -180 ~ 180의 값으로 변환하여 RotationDiff에 준다.
	(TargetRelativeRotation - CurRelativeRotation).GetWindingAndRemainder(Dummyrot,RotationDiff);

	FRotator NewRelativeRotation = CurRelativeRotation;

	//만약 거의 값이 같아서 돌릴필요가 없으면 돌리지 않는다.
	if(RotationDiff.IsNearlyZero(0.001f))
	{
		bLocked = true;
	}
	else
	{
		bLocked = false;
		//실제로 돌릴각도 (빈 공간)
		FRotator DeltaRotation = FRotator::ZeroRotator;

		//돌릴 Pitch값을 정한다.
		if(FMath::Abs(RotationDiff.Pitch)<=PitchRotationSpeed*PitchRotationSpeedMultiplier*DeltaTime)
		{
			//만약 회전시켜야 하는 총각도가 회전속도*DeltaTime 보다 작으면, 그대로 돌린다.
			DeltaRotation.Pitch = RotationDiff.Pitch;
		}
		else
		{
			//회전시켜야 하는 총각도가 회전속도*DeltaTime보다 크면, 회전속도에 맞게끔 방향만 맞춰서 돌린다.
			DeltaRotation.Pitch = FMath::Sign(RotationDiff.Pitch)*PitchRotationSpeed*PitchRotationSpeedMultiplier*DeltaTime;
		}

		//Yaw도 마찬가지로 돌릴각도를 정한다.
		if(FMath::Abs(RotationDiff.Yaw)<=YawRotationSpeed*YawRotationSpeedMultiplier*DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw)*YawRotationSpeed*YawRotationSpeedMultiplier*DeltaTime;
		}

		//현재상대회전 + 돌릴각도 = 새 상대회전
		//굳이 이렇게 한단계 거치는 이유는 본체나 포탑이 너무 빠르게 회전할 경우 Roll값이 조금씩 돌아가는 문제가 있기 때문입니다.
		//부동소수점 연산에서 생기는 찌꺼기 값이 계속 누적되서 일지도...
		NewRelativeRotation = CurRelativeRotation + DeltaRotation;
		NewRelativeRotation.Pitch = FMath::ClampAngle(NewRelativeRotation.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]);
		NewRelativeRotation.Yaw = FMath::ClampAngle(NewRelativeRotation.Yaw,YawRotationLimit[0],YawRotationLimit[1]);
		NewRelativeRotation.Roll = 0.0f;
		SetActorRelativeRotation(NewRelativeRotation);
	}
	
	FRotator TargetAngleDiff;
	(RelativeTargetDirection - NewRelativeRotation).GetWindingAndRemainder(Dummyrot,RotationDiff);

	if(TargetAngleDiff.IsNearlyZero(0.001f))
	{
		bIsOnTarget = true;
	}
	else
	{
		bIsOnTarget = false;
	}
}