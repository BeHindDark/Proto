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
	//WeaponControlSystem ���� Ȯ��
	if(!IsValid(WeaponControlSystem_Ref))
	{
		//�� ƽ���� ȣ��Ǵ� �Լ��̱⿡ �αװ� �ʹ� �������Ƿ� ���� �α״� ���� �ʽ��ϴ�.
		return;
	}

	//���� ���� Ȯ��
	if(!SocketArrow_Ref.IsValid())
	{
		//�� ƽ���� ȣ��Ǵ� �Լ��̱⿡ �αװ� �ʹ� �������Ƿ� ���� �α״� ���� �ʽ��ϴ�.
		return;
	}

	//WCS���� ���� ��ǥ������ �޾� ����
	TargetLocation = WeaponControlSystem_Ref->TargetWorldLocation;

	//ArrowComponent�� �̿��� ���� �߽� ��ǥ�踦 ���մϴ�.
	FTransform SocketTransform = SocketArrow_Ref.Get()->GetComponentTransform();

	//�������� �߻� ���͸� ������ ����
	FVector LaunchVelocity;

	//�߻�ӵ�(��������)�� ���մϴ�.
	if(!UGameplayStatics::SuggestProjectileVelocity(this,LaunchVelocity,GetActorLocation(),TargetLocation,LaunchSpeed,false,0.0f,0.0F,ESuggestProjVelocityTraceOption::DoNotTrace))
	{
		TurnTowardDirectAim(DeltaTime);
		return;
	}

	/**	���� ���� ��ǥ�迡�� Ÿ���� ���߱� ���� ����ü�� �߻��ؾ� �ϴ� ����
	*/
	FRotator RelativeLaunchDirection = FTransform(LaunchVelocity.ToOrientationRotator(), SocketTransform.GetLocation()).GetRelativeTransform(SocketTransform).Rotator();


	/**	��ǥ������ �ٶ󺸱� ���� ��ž�� �������ϴ� ���ȸ����
		ClampAngle�� ���� �ִ� �ξӰ��� ���ѽ�Ų��.
		Roll�� 0���� �ξ� ��ž�� �¿�� �䵿ġ�� �ʰ� �Ѵ�.
	*/
	FRotator TargetRelativeRotation = FRotator(FMath::ClampAngle(RelativeLaunchDirection.Pitch,PitchRotationLimit[0],PitchRotationLimit[1]),
											   FMath::ClampAngle(RelativeLaunchDirection.Yaw,YawRotationLimit[0],YawRotationLimit[1]),
											   0.0f);

   //���Ͽ� ���� ��ž�� ���� rotation
	FRotator CurRelativeRotation = GetActorTransform().GetRelativeTransform(SocketTransform).Rotator();

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

