// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_WeaponBase.h"
#include "Components/ArrowComponent.h"
#include "WeaponControlSystem.h"

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
	//SingleMuzzleArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("MuzzleArrow"));

	RootComponent = DefaultSceneRoot;
}

void AAct_WeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAct_WeaponBase,TargetLocation);
}

// Called when the game starts or when spawned
void AAct_WeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
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
}

void AAct_WeaponBase::ServerOnCeaseFireOrder()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	//���� ���� ���⿡ �°� �������̵� �Ͽ� ����ϸ� �˴ϴ�.
}

void AAct_WeaponBase::ConnectWeaponControlSystem(UWeaponControlSystem * NewWeaponControlSystem,int NewWeaponIndex)
{
	WeaponControlSystem_Ref = TWeakObjectPtr<UWeaponControlSystem>(NewWeaponControlSystem);

	SocketArrow_Ref = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].SocketArrow_Ref;

	WeaponIndex = NewWeaponIndex;

	PitchRotationLimit = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].PitchRotationLimit;

	YawRotationLimit = NewWeaponControlSystem->WeaponDataArray[NewWeaponIndex].PitchRotationLimit;
}

void AAct_WeaponBase::SetSingleMuzzleArrow(UArrowComponent * MuzzleArrow)
{
	SingleMuzzleArrow = MuzzleArrow;
}

void AAct_WeaponBase::TurnTowardDirectAim(float DeltaTime)
{
	if(!WeaponControlSystem_Ref.IsValid())
	{
		return;
	}

	if(!SocketArrow_Ref.IsValid())
	{
		return;
	}

	TargetLocation = WeaponControlSystem_Ref.Get()->TargetWorldLocation;

	FTransform SocketTransform = SocketArrow_Ref.Get()->GetComponentTransform();

	//������ �������� ���� �� Ÿ���� ��ġ�� ���� ��
	//ToOrientationRotator�� �ش� ��ġ�� �ٶ󺸵��� �ϴ� rotation�� ���Ѵ�. �̶� roll=0���� �����ȴ�.
	FRotator RelativeTargetDirection = FTransform(FRotator::ZeroRotator,TargetLocation).GetRelativeTransform(SocketTransform).GetLocation().ToOrientationRotator();

	//��ǥ������ �ٶ󺸱� ���� ��ž�� �������ϴ� ���ȸ����
	//ClampAngle�� ���� �ִ� �ξӰ��� ���ѽ�Ų��.

	FRotator TargetRelativeRotation = FRotator(FMath::ClampAngle(RelativeTargetDirection.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]),
											   FMath::ClampAngle(RelativeTargetDirection.Yaw,YawRotationLimit[0],YawRotationLimit[1]),
											   0.0f);

   //���Ͽ� ���� ��ž�� ���� rotation
	FRotator CurRelativeRotation = GetActorTransform().GetRelativeTransform(SocketTransform).Rotator();

	//Ÿ���� �ٶ󺸰� �ϱ� ���� ȸ�����Ѿ� �ϴ� �Ѱ���
	FRotator RotationDiff = FRotator::ZeroRotator;
	//����� �� ���� ����
	FRotator Dummyrot = FRotator::ZeroRotator;

	//�� ������ ���̸� ���� �� -180 ~ 180�� ������ ��ȯ�Ͽ� RotationDiff�� �ش�.
	(TargetRelativeRotation - CurRelativeRotation).GetWindingAndRemainder(Dummyrot,RotationDiff);

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
		if(FMath::Abs(RotationDiff.Pitch)<=PitchRotationSpeed*DeltaTime)
		{
			//���� ȸ�����Ѿ� �ϴ� �Ѱ����� ȸ���ӵ�*DeltaTime ���� ������, �״�� ������.
			DeltaRotation.Pitch = RotationDiff.Pitch;
		}
		else
		{
			//ȸ�����Ѿ� �ϴ� �Ѱ����� ȸ���ӵ�*DeltaTime���� ũ��, ȸ���ӵ��� �°Բ� ���⸸ ���缭 ������.
			DeltaRotation.Pitch = FMath::Sign(RotationDiff.Pitch)*PitchRotationSpeed*DeltaTime;
		}

		//Yaw�� ���������� ���������� ���Ѵ�.
		if(FMath::Abs(RotationDiff.Yaw)<=PitchRotationSpeed*DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw)*YawRotationSpeed*DeltaTime;
		}

		//������ȸ�� + �������� = �� ���ȸ��
		//���� �̷��� �Ѵܰ� ��ġ�� ������ ��ü�� ��ž�� �ʹ� ������ ȸ���� ��� Roll���� ���ݾ� ���ư��� ������ �ֱ� �����Դϴ�.
		//�ε��Ҽ��� ���꿡�� ����� ��� ���� ��� �����Ǽ� ������...
		FRotator NewRelativeRotation = CurRelativeRotation + DeltaRotation;
		NewRelativeRotation.Pitch = FMath::ClampAngle(NewRelativeRotation.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]);
		NewRelativeRotation.Yaw = FMath::ClampAngle(NewRelativeRotation.Yaw,YawRotationLimit[0],YawRotationLimit[1]);
		NewRelativeRotation.Roll = 0.0f;
		SetActorRelativeRotation(NewRelativeRotation);
	}
}