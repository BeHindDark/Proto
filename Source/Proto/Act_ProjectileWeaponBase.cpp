// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_ProjectileWeaponBase.h"
#include "WeaponControlSystem.h"
#include "Components/ArrowComponent.h"
#include "Act_Bullet.h"

AAct_ProjectileWeaponBase::AAct_ProjectileWeaponBase()
{
	WeaponSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	MuzzleFrameParticle = CreateDefaultSubobject<UParticleSystem>(TEXT("MuzzleFrameParticleSystem"));

	WeaponSkeletalMesh->SetupAttachment(DefaultSceneRoot);
	WeaponSkeletalMesh->SetRelativeRotation(FRotator(90,-90,0));
	WeaponSkeletalMesh->SetCollisionProfileName(TEXT("HitBox"));
	WeaponSkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	

}

void AAct_ProjectileWeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAct_ProjectileWeaponBase,IsTriggerOn);
}

void AAct_ProjectileWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void AAct_ProjectileWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TurnTowardProjectileAim(DeltaTime);
}

void AAct_ProjectileWeaponBase::TurnTowardProjectileAim(float DeltaTime)
{
	//WeaponControlSystem 연결 확인
	if(!IsValid(WeaponControlSystem_Ref))
	{
		//매 틱마다 호출되는 함수이기에 로그가 너무 많아지므로 따로 로그는 넣지 않습니다.
		return;
	}

	//소켓 연결 확인
	if(!SocketArrow_Ref.IsValid())
	{
		//매 틱마다 호출되는 함수이기에 로그가 너무 많아지므로 따로 로그는 넣지 않습니다.
		return;
	}

	//WCS으로 부터 목표지점을 받아 저장
	TargetLocation = WeaponControlSystem_Ref->TargetWorldLocation;

	//ArrowComponent를 이용해 소켓 중심 좌표계를 구합니다.
	FTransform SocketTransform = SocketArrow_Ref.Get()->GetComponentTransform();

	//최종적인 발사 벡터를 저장할 변수
	FVector LaunchVelocity;

	//발사속도(방향포함)를 구합니다.
	if(!UGameplayStatics::SuggestProjectileVelocity(this,LaunchVelocity,GetActorLocation(),TargetLocation,LaunchSpeed,false,0.0f,0.0F,ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		TurnTowardDirectAim(DeltaTime);
		return;
	}

	/**	소켓 기준 좌표계에서 타겟을 맞추기 위해 투사체를 발사해야 하는 방향
	*/
	FRotator RelativeLaunchDirection = FTransform(LaunchVelocity.ToOrientationRotator(), SocketTransform.GetLocation()).GetRelativeTransform(SocketTransform).Rotator();


	/**	목표지점을 바라보기 위해 포탑이 가져야하는 상대회전값
		ClampAngle을 통해 최대 부앙각을 제한시킨다.
		Roll은 0으로 두어 포탑이 좌우로 요동치지 않게 한다.
	*/
	FRotator TargetRelativeRotation = FRotator(FMath::ClampAngle(RelativeLaunchDirection.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]),
											   FMath::ClampAngle(RelativeLaunchDirection.Yaw,YawRotationLimit[0],YawRotationLimit[1]),
											   0.0f);

   //소켓에 대한 포탑의 현재 rotation
	FRotator CurRelativeRotation = GetActorTransform().GetRelativeTransform(SocketTransform).Rotator();

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
		if(FMath::Abs(RotationDiff.Pitch)<=PitchRotationSpeed*DeltaTime)
		{
			//만약 회전시켜야 하는 총각도가 회전속도*DeltaTime 보다 작으면, 그대로 돌린다.
			DeltaRotation.Pitch = RotationDiff.Pitch;
		}
		else
		{
			//회전시켜야 하는 총각도가 회전속도*DeltaTime보다 크면, 회전속도에 맞게끔 방향만 맞춰서 돌린다.
			DeltaRotation.Pitch = FMath::Sign(RotationDiff.Pitch)*PitchRotationSpeed*DeltaTime;
		}

		//Yaw도 마찬가지로 돌릴각도를 정한다.
		if(FMath::Abs(RotationDiff.Yaw)<=PitchRotationSpeed*DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw)*YawRotationSpeed*DeltaTime;
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
	(RelativeLaunchDirection - NewRelativeRotation).GetWindingAndRemainder(Dummyrot,RotationDiff);

	if(bIsOnTarget!=TargetAngleDiff.IsNearlyZero(0.001f))
	{
		bIsOnTarget=(!bIsOnTarget);
	}
}

void AAct_ProjectileWeaponBase::SpawnBulletInServer(FVector MuzzleLocation, FRotator LaunchRotation)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	if(GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		//SpawnParams.Instigator = this->Instigator;
		AAct_Bullet* Bullet = GetWorld()->SpawnActor<AAct_Bullet>(AAct_Bullet::StaticClass(), MuzzleLocation, LaunchRotation, SpawnParams);
		
		if(IsValid(InstigatorController))
		{
			Bullet->InitializeBullet(this,InstigatorController,LaunchSpeed,Damage*DamageMultiplier,TracerColor);
		}
		else
		{
			Bullet->InitializeBullet(this,nullptr, LaunchSpeed, Damage*DamageMultiplier, TracerColor);
		}
	}
}

void AAct_ProjectileWeaponBase::MulticastFireFX_Implementation(UArrowComponent* MuzzleArrow)
{
	if(GetWorld())
	{
		if(IsValid(MuzzleFrameParticle))
		{
			if(IsValid(MuzzleArrow))
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),MuzzleFrameParticle,MuzzleArrow->GetComponentLocation())
					->AttachToComponent(MuzzleArrow,FAttachmentTransformRules::KeepRelativeTransform);
			}

		}
	}
}

