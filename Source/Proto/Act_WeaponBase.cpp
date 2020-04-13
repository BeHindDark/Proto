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

	//���ø����̼� ����
	bReplicates = true;
	bAlwaysRelevant = true;
	SetReplicatingMovement(true);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));

	RootComponent = DefaultSceneRoot;
		
}

void AAct_WeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	//���� ���ø����̼��� �� �� �׻� �����ؾ� �ϴ� �Լ��Դϴ�.
	//������ ���� ���Ǻκи� �־��ָ� �˴ϴ�.
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
	//���� ���� ���⿡ �°� �������̵� �Ͽ� ����ϸ� �˴ϴ�.
	UE_LOG(Proto,Warning,TEXT("%s / %s : FireOrder"),*LINE_INFO,*GetNameSafe(this));
}

void AAct_WeaponBase::ServerOnCeaseFireOrder()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	//���� ���� ���⿡ �°� �������̵� �Ͽ� ����ϸ� �˴ϴ�.
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

	//������ �������� ���� �� Ÿ���� ��ġ�� ���� ��
	//ToOrientationRotator�� �ش� ��ġ�� �ٶ󺸵��� �ϴ� rotation�� ���Ѵ�. �̶� roll=0���� �����ȴ�.
	FRotator RelativeTargetDirection = FTransform(FRotator::ZeroRotator,TargetLocation).GetRelativeTransform(SocketTransform).GetLocation().ToOrientationRotator();

	RelativeTargetDirection.Roll = 0.0f;

	//���Ͽ� ���� ��ž�� ���� rotation
	FRotator CurRelativeRotation = GetActorTransform().GetRelativeTransform(SocketTransform).Rotator();
	

	//��ǥ������ �ٶ󺸱� ���� ��ž�� �������ϴ� ���ȸ����
	//ClampAngle�� ���� �ִ� �ξӰ��� ���ѽ�Ų��.

	FRotator TargetRelativeRotation = FRotator(FMath::ClampAngle(RelativeTargetDirection.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]),
											   FMath::ClampAngle(RelativeTargetDirection.Yaw,YawRotationLimit[0],YawRotationLimit[1]),
											   0.0f);
	   

	//Ÿ���� �ٶ󺸰� �ϱ� ���� ȸ�����Ѿ� �ϴ� �Ѱ���
	FRotator RotationDiff = FRotator::ZeroRotator;
	//����� �� ���� ����
	FRotator Dummyrot = FRotator::ZeroRotator;

	//�� ������ ���̸� ���� �� -180 ~ 180�� ������ ��ȯ�Ͽ� RotationDiff�� �ش�.
	(TargetRelativeRotation - CurRelativeRotation).GetWindingAndRemainder(Dummyrot,RotationDiff);

	FRotator NewRelativeRotation = CurRelativeRotation;

	//���� ���� ���� ���Ƽ� �����ʿ䰡 ������ ������ �ʴ´�.
	if(RotationDiff.IsNearlyZero(0.001f))
	{
		bLocked = true;
	}
	else
	{
		bLocked = false;
		//������ �������� (�� ����)
		FRotator DeltaRotation = FRotator::ZeroRotator;

		//���� Pitch���� ���Ѵ�.
		if(FMath::Abs(RotationDiff.Pitch)<=PitchRotationSpeed*PitchRotationSpeedMultiplier*DeltaTime)
		{
			//���� ȸ�����Ѿ� �ϴ� �Ѱ����� ȸ���ӵ�*DeltaTime ���� ������, �״�� ������.
			DeltaRotation.Pitch = RotationDiff.Pitch;
		}
		else
		{
			//ȸ�����Ѿ� �ϴ� �Ѱ����� ȸ���ӵ�*DeltaTime���� ũ��, ȸ���ӵ��� �°Բ� ���⸸ ���缭 ������.
			DeltaRotation.Pitch = FMath::Sign(RotationDiff.Pitch)*PitchRotationSpeed*PitchRotationSpeedMultiplier*DeltaTime;
		}

		//Yaw�� ���������� ���������� ���Ѵ�.
		if(FMath::Abs(RotationDiff.Yaw)<=YawRotationSpeed*YawRotationSpeedMultiplier*DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw)*YawRotationSpeed*YawRotationSpeedMultiplier*DeltaTime;
		}

		//������ȸ�� + �������� = �� ���ȸ��
		//���� �̷��� �Ѵܰ� ��ġ�� ������ ��ü�� ��ž�� �ʹ� ������ ȸ���� ��� Roll���� ���ݾ� ���ư��� ������ �ֱ� �����Դϴ�.
		//�ε��Ҽ��� ���꿡�� ����� ��� ���� ��� �����Ǽ� ������...
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