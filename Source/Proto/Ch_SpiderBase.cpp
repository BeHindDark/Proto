// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_SpiderBase.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponControlSystem.h"
#include "DamageControlSystem.h"
#include "Act_WeaponBase.h"

// Sets default values
ACh_SpiderBase::ACh_SpiderBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//�⺻��(������Ʈ ���, ��ǥ, ��Ÿ �⺻ ��) ����
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	WeaponControlSystem = CreateDefaultSubobject<UWeaponControlSystem>(TEXT("WCS"));
	DamageControlSystem = CreateDefaultSubobject<UDamageControlSystem>(TEXT("DCS"));
		
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 30.0f, 0.0f);

	WeaponControlSystem->SetIsReplicated(true);
	DamageControlSystem->SetIsReplicated(true);

}

/**	���� ���ø����̼��� �� �� �׻� �����ؾ� �ϴ� �Լ��Դϴ�.
*	������ ���� ���Ǻκи� �־��ָ� �˴ϴ�.
*	�������� �����ϼ���
*/
void ACh_SpiderBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
		
	//UPROPERTY(replicated,VisibleAnywhere,BlueprintReadWrite,Category = "Spider|WeaponControlSystem")
	//FVector AimLocation = FVector::ZeroVector;
	//DOREPLIFETIME(ACh_SpiderBase,AimLocation);
	
	DOREPLIFETIME(ACh_SpiderBase,bIsPlayerControlling);
	DOREPLIFETIME(ACh_SpiderBase,PlayerController);
}

// Called when the game starts or when spawned
void ACh_SpiderBase::BeginPlay()
{
	Super::BeginPlay();	
	DamageControlSystem->StartSystem();
}

void ACh_SpiderBase::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	/*
	if(IsPlayerControlled())
	{
		PlayerController = TWeakObjectPtr<APlayerController>(Cast<APlayerController>(NewController));
		if(PlayerController.IsValid())
		{
			bIsPlayerControlling = true;
		}
	}
	*/
}

void ACh_SpiderBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	AimLocation = GetActorLocation() + 100.0f*GetActorForwardVector();
	
	WeaponControlSystem->InitializeWeaponNumber(WeaponSlotNum);
	DamageControlSystem->InitializeSystem(MaxHP);
	DamageControlSystem->OnHPIsZero.AddUObject(this, &ACh_SpiderBase::OnHPIsZero);
	
	//DeathAnimAssset�� ��� �����°� �˸��� notify�� ���� �� ��ü�� �ı���Ű�� ������ �߰�����.
}

// Called every frame
void ACh_SpiderBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(!bIsLocalPlayerControlled)
	{
		if(IsPlayerControlled())
		{
			if(IsLocallyControlled())
			{
				bIsLocalPlayerControlled = true;
			}
		}
	}
	else
	{
		if(IsValid(Camera)&&IsValid(SpringArm))
		{
			//ī�޶� ȸ��
			Camera->AddRelativeRotation(FRotator(CameraPitchMovement * DeltaTime,0.0f,0.0f));
			SpringArm->AddRelativeRotation(FRotator(0.0f,CameraYawMovement * DeltaTime,0.0f));
		}
	}

	if(IsValid(WaistSceneComponent))
	{
		TurnUpperBody(WaistSceneComponent,DeltaTime);
	}

	//DeltaTime�� �� 0.003~0.005�Դϴ�.
	TickTimeStack += DeltaTime;

	if(TickTimeStack>=NetworkTickInterval)
	{
		//��Ʈ��ũ �Լ� ����
		NetworkTick(TickTimeStack);

		TickTimeStack = 0.0f;
	}
}

// Called to bind functionality to input
void ACh_SpiderBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ACh_SpiderBase::TurnCamera);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this,&ACh_SpiderBase::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this,&ACh_SpiderBase::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this,&ACh_SpiderBase::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnBody"),this,&ACh_SpiderBase::TurnBody);


	//���� ���� �׼� ���ε�
	//���ε��� �Է� �̸� ����
	TArray<FName> WeaponGroupName ={FName(TEXT("WeaponGroup1")), FName(TEXT("WeaponGroup2")), FName(TEXT("WeaponGroup3"))};
	for(int i=0; i<3; i++)
	{
		//���̵��� �׼� �̸��� Ű�� �׼ǹ��ε� ����
		FInputActionBinding WeaponGroupActionBinding(WeaponGroupName[i],IE_Pressed);

		//��������Ʈ ����
		FInputActionHandlerSignature WeaponGroupActionHandler;

		//��������Ʈ�� �Լ� ���ε�
		WeaponGroupActionHandler.BindUFunction(this,FName("ChangeWeaponGroup"),i);

		//�׼ǹ��ε��� ��������Ʈ ����
		WeaponGroupActionBinding.ActionDelegate = WeaponGroupActionHandler;

		//�ϼ��� �׼ǹ��ε��� InputComponent�� �߰�
		PlayerInputComponent->AddActionBinding(WeaponGroupActionBinding);
	}

	PlayerInputComponent->BindAction(FName(TEXT("Fire")),IE_Pressed,this,&ACh_SpiderBase::OnTriggerDown);
	PlayerInputComponent->BindAction(FName(TEXT("Fire")),IE_Released,this,&ACh_SpiderBase::OnTriggerUp);
}

FVector ACh_SpiderBase::GetCameraAimLocation(UCameraComponent* CurrentCamera)
{
	FVector AimEnd = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 10000.0f);
	FCollisionQueryParams AimParams;
	FHitResult AimResult;
	//Actors to ignore�� spider�� ������ �����, ���ø�����Ʈ �ǰ� �Ѵ�.
	//���� Weapon�� attach��ų �� ignore�� �߰���Ű�� ������ �����.(WCS����)
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for(FWeaponData WeaponData:WeaponControlSystem->WeaponDataArray)
	{
		ActorsToIgnore.Add(WeaponData.Weapon);
	}

	AimParams.AddIgnoredActors(ActorsToIgnore);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(AimResult,Camera->GetComponentLocation(),AimEnd,ECC_Visibility,AimParams);
	if(IsHit)
	{
		if(AimResult.bBlockingHit)
		{
			return FVector(AimResult.ImpactPoint);
		}
		else
		{
			return AimEnd;
		}
	}
	else
	{
		return AimEnd;
	}
}

void ACh_SpiderBase::TurnCamera(float NewAxisValue)
{
	CameraYawMovement = CameraYawSpeed * NewAxisValue;
}

void ACh_SpiderBase::LookUp(float NewAxisValue)
{
	CameraPitchMovement = CameraPitchSpeed * -NewAxisValue;
}

void ACh_SpiderBase::MoveForward(float NewAxisValue)
{
	MoveInput = NewAxisValue;
	AddMovementInput(GetActorForwardVector(),NewAxisValue);
}

void ACh_SpiderBase::MoveRight(float NewAxisValue)
{
	if(FMath::IsNearlyEqual(MoveInput,0.0f,0.01f))
	{
		AddMovementInput(GetActorRightVector(),NewAxisValue);
	}
	else
	{
		if(MoveInput > 0.0f)
		{
			AddControllerYawInput(NewAxisValue * BodyYawSpeed);
		}
		else
		{
			AddControllerYawInput(-NewAxisValue * BodyYawSpeed);
		}
	}
}

void ACh_SpiderBase::TurnBody(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue * BodyYawSpeed);
}

void ACh_SpiderBase::ChangeWeaponGroup(int NewGroup)
{
	
	if(NewGroup>=WeaponSlotNum)
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : The Index is out of range. Fail to change active weapon group."),*LINE_INFO,*GetNameSafe(this));
		return;
	}
	if(!IsValid(WeaponControlSystem))
	{
		return;
	}
	
	WeaponControlSystem->ActivateWeaponGroup(NewGroup);
}

void ACh_SpiderBase::OnTriggerDown_Implementation()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}

	WeaponControlSystem->SendFireOrder();
}

bool ACh_SpiderBase::OnTriggerDown_Validate()
{
	return true;
}

void ACh_SpiderBase::OnTriggerUp_Implementation()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}

	WeaponControlSystem->SendCeaseFireOrder();
}

bool ACh_SpiderBase::OnTriggerUp_Validate()
{
	return true;
}

void ACh_SpiderBase::SetWaistSceneComponent(USceneComponent * BlueprintWaistSceneComponent)
{
	if(IsValid(BlueprintWaistSceneComponent))
	{
		WaistSceneComponent = BlueprintWaistSceneComponent;
	}
}

void ACh_SpiderBase::TurnUpperBody(USceneComponent* WaistComponent,float DeltaTime)
{
	//�����ǥ�� �󿡼� ��ǥ�� ����
	FRotator RelativeTargetDirection;

	//���� ����� ���Ͽ� �پ��ִٸ� ���ϱ��� ��ǥ��, �ƴϸ� ���� ���� ��ǥ�踦 �̿�
	if(!WaistComponent->GetAttachSocketName().IsNone())
	{
		const USkeletalMeshSocket* WaistSocket = GetMesh()->GetSocketByName(WaistComponent->GetAttachSocketName());
		RelativeTargetDirection = FTransform(FRotator::ZeroRotator,AimLocation).GetRelativeTransform(WaistSocket->GetSocketTransform(GetMesh())).GetLocation().ToOrientationRotator();
	}
	else
	{
		RelativeTargetDirection = FTransform(FRotator::ZeroRotator,AimLocation).GetRelativeTransform(GetActorTransform()).GetLocation().ToOrientationRotator();
	}

	//��ž�� �������ϴ� ���ȸ��
	FRotator TargetUpperBodyRotation = FRotator(0.0f,RelativeTargetDirection.Yaw,0.0f);

	//���� ��ü�� ���ȸ��
	FRotator CurrentUpperBodyRotation = WaistComponent->GetRelativeTransform().Rotator();

	//���� ȸ���� ��ǥ ȸ���� ���̰�
	FRotator RotationDiff = FRotator::ZeroRotator;

	//������ ���̰�
	FRotator DummyRot = FRotator::ZeroRotator;

	//�� �Լ��� �̿��� Rotation�� Pitch, yaw, Roll�� -180 ~ +180�� ���� ������ �Ѵ�.
	(TargetUpperBodyRotation - CurrentUpperBodyRotation).GetWindingAndRemainder(DummyRot,RotationDiff);

	//ȸ���� ���̰� �ʹ� ������ ������ �ʴ´�.
	if(!RotationDiff.IsNearlyZero(0.001f))
	{
		//�̹� ƽ�� �������ϴ� �����̼�
		FRotator DeltaRotation = FRotator::ZeroRotator;
		if(FMath::Abs(RotationDiff.Yaw) <= UpperBodyRotationSpeed * DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw) * UpperBodyRotationSpeed * DeltaTime;
		}

		//���ο� �����̼��� ������, ��Ⱚ�� ���̴� �� �����ϱ� ���� pich�� roll�� 0���� �����ȴ�.
		FRotator NewUpperBodyRotation = CurrentUpperBodyRotation + DeltaRotation;
		
		//����
		WaistSceneComponent->SetRelativeRotation(FRotator(0.0f, NewUpperBodyRotation.Yaw, 0.0f));
	}
}

void ACh_SpiderBase::Multicast_SetUpperBodyYaw_Implementation(float NewYaw)
{
	if(IsValid(WaistSceneComponent))
	{
		WaistSceneComponent->SetRelativeRotation(FRotator(0.0f,NewYaw,0.0f));
	}	
}

float ACh_SpiderBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage >= 0.0f) {
		//������ ������ ü�°���
		CurrentHP -= Damage;
		//������ ü�� �Ѱ��ִ� �κ�
		//������ ����ؼ� � ������ HP���� �Ű������� �Ѱ���
		/*
		if(IsValid(WG_InGame))
		{
			WG_InGame->SetHP(HPType::BODY,(int)CurrentHP);
		}
		*/
		if (CurrentHP <= 0.0f) {
			OnDeath.Broadcast(EventInstigator);
			//���⿡ ĳ���� ����Լ� Ȥ�� �й���� �Լ��� ���ߵ˴ϴ�
			DeathAnim();
		}
	}
	return ActualDamage;
}


void ACh_SpiderBase::OnWeaponTakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Damage >= 0.0f) {
		CurrentHP -= Damage;
		/*
		if(IsValid(WG_InGame))
		{
			WG_InGame->SetHP(HPType::BODY,(int)CurrentHP);
		}
		*/
		if (CurrentHP <= 0.0f) {

			OnDeath.Broadcast(EventInstigator);
			//���⿡ ĳ���� ����Լ� ���ߵ˴ϴ�
			CurrentHP = 0.0f;
			DeathAnim();
		}
	}
}

void ACh_SpiderBase::OnHPIsZero(AController* DamageInstigator,AActor* DamageCauser,AActor* DamageReciever)
{
	if(GetLocalRole()>=ROLE_Authority)
	{
		OnDeath.Broadcast(DamageInstigator);
	}	
}

void ACh_SpiderBase::DeathAnim_Implementation()
{
	if(IsValid(DeathAnimAsset))
	{
		GetMesh()->PlayAnimation(DeathAnimAsset, false);
	}
}

void ACh_SpiderBase::NetworkTick(float NetworkDeltaTime)
{
	if(bIsLocalPlayerControlled)
	{
		if(IsValid(Camera))
		{
			FVector NewLocalAim = GetCameraAimLocation(Camera);
			ServerNetTick(NewLocalAim);
		}
	}

	if(GetLocalRole()>=ROLE_Authority)
	{
		MulticastNetTick(AimLocation);
	}
}

void ACh_SpiderBase::ServerNetTick_Implementation(FVector LocalAim)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	AimLocation = LocalAim;
}

void ACh_SpiderBase::MulticastNetTick_Implementation(FVector ServerAim)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		AimLocation = ServerAim;
	}
	WeaponControlSystem->TargetWorldLocation = ServerAim;
}