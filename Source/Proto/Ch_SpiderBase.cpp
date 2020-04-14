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
#include "Act_ProjectileWeaponBase.h"
#include "WG_InGame_Information.h"

// Sets default values
ACh_SpiderBase::ACh_SpiderBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	//�⺻��(������Ʈ ���, ��ǥ, ��Ÿ �⺻ ��) ����
	//WaistSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("WaistSceneComponent"));
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	WCS = CreateDefaultSubobject<UWeaponControlSystem>(TEXT("WCS"));
		
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 30.0f, 0.0f);

	if(GetLocalRole()>=ROLE_Authority)
	{
		AimLocation = GetActorLocation() + 100.0f*GetActorForwardVector();
	}
	
	WCS->SetIsReplicated(true);

	
}

/**	���� ���ø����̼��� �� �� �׻� �����ؾ� �ϴ� �Լ��Դϴ�.
*	������ ���� ���Ǻκи� �־��ָ� �˴ϴ�.
*/
void ACh_SpiderBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACh_SpiderBase,AimLocation);
}

// Called when the game starts or when spawned
void ACh_SpiderBase::BeginPlay()
{
	Super::BeginPlay();	

	CurrentHP = MaxHP;
}

void ACh_SpiderBase::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	
	if(IsPlayerControlled())
	{
		PlayerController = Cast<APlayerController>(NewController);
		if(PlayerController != nullptr)
		{
			bIsPlayerControlling = true;
		}
	}
}

void ACh_SpiderBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WCS->InitializeWeaponNumber(WeaponSlotNum);

}

// Called every frame
void ACh_SpiderBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if(IsPlayerControlled())
	{
		if(IsValid(GetController()))
		{
			if(GetController()->IsLocalController())
			{
				if(Camera != nullptr)
				{
					Camera->AddRelativeRotation(FRotator(CameraPitchMovement * DeltaTime,0.0f,0.0f));
				}
				if(SpringArm != nullptr)
				{
					SpringArm->AddRelativeRotation(FRotator(0.0f,CameraYawMovement * DeltaTime,0.0f));
				}
				FVector NewLocalAim = CameraAimLocation(Camera);
				AimLocation = NewLocalAim;
				WCS->TargetWorldLocation = NewLocalAim;
				ServerNetTick(NewLocalAim,DeltaTime);
			}
			
		}
	}
	if(IsValid(WaistSceneComponent))
	{
		TurnUpperBody(WaistSceneComponent,DeltaTime);
	}
}

// Called to bind functionality to input
void ACh_SpiderBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("Turn"),this,&ACh_SpiderBase::Turn);
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
	//OnTakePointDamage.AddDynamic()
}

FVector ACh_SpiderBase::CameraAimLocation(UCameraComponent* CurrentCamera) {
	FVector AimPoint = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 10000.0f);
	FCollisionQueryParams AimParams;
	FHitResult AimResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for(FWeaponData WeaponData:WCS->WeaponDataArray) {
		ActorsToIgnore.Add(WeaponData.Weapon);
	}
	AimParams.AddIgnoredActors(ActorsToIgnore);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(AimResult,Camera->GetComponentLocation(),AimPoint,ECC_Visibility,AimParams);
	if(IsHit) {
		if(AimResult.bBlockingHit) {
			return FVector(AimResult.ImpactPoint);
		}
		else {
			return AimPoint;
		}
	}
	else {
		return AimPoint;
	}
}

void ACh_SpiderBase::Turn(float NewAxisValue) {
	CameraYawMovement = CameraYawSpeed * NewAxisValue;
}

void ACh_SpiderBase::LookUp(float NewAxisValue) {
	CameraPitchMovement = CameraPitchSpeed * -NewAxisValue;
}

void ACh_SpiderBase::MoveForward(float NewAxisValue) {
	MoveInput = NewAxisValue;
	AddMovementInput(GetActorForwardVector(),NewAxisValue);
}

void ACh_SpiderBase::MoveRight(float NewAxisValue) {
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
	//IsLocallyControlled()

	if(NewGroup>=WeaponSlotNum)
	{
		UE_LOG(Proto,Warning,TEXT("%s / %s : The Index is out of range. Fail to change active weapon group."),*LINE_INFO,*GetNameSafe(this));
		return;
	}
	if(!IsValid(WCS))
	{
		return;
	}

	WCS->ActivateWeaponGroup(NewGroup);
}

void ACh_SpiderBase::OnTriggerDown_Implementation()
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}

	WCS->SendFireOrder();
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

	WCS->SendCeaseFireOrder();
}

bool ACh_SpiderBase::OnTriggerUp_Validate()
{
	return true;
}
void ACh_SpiderBase::SetWaistSceneComponent(USceneComponent * BlueprintWaistSceneComponent)
{
	WaistSceneComponent = BlueprintWaistSceneComponent;
}


void ACh_SpiderBase::ServerNetTick_Implementation(FVector CameraAim,float Deltatime)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	AimLocation = CameraAim;
	WCS->TargetWorldLocation = AimLocation;
	
}

bool ACh_SpiderBase::ServerNetTick_Validate(FVector CameraAim,float Deltatime)
{
	return true;
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

		FRotator NewUpperBodyRotation = CurrentUpperBodyRotation + DeltaRotation;
		NewUpperBodyRotation.Pitch = 0.0f;
		NewUpperBodyRotation.Roll = 0.0f;
		WaistComponent->SetRelativeRotation(NewUpperBodyRotation);
	}
}
float ACh_SpiderBase::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, class AActor* DamageCauser) {
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	
	if (ActualDamage >= 0.0f) {
		//������ ������ ü�°���
		CurrentHP -= Damage;
		//������ ü�� �Ѱ��ִ� �κ�
		//������ ����ؼ� � ������ HP���� �Ű������� �Ѱ���
		if(IsValid(WG_InGame))
		{
			WG_InGame->SetHP(HPType::BODY,(int)CurrentHP);
		}		
		if (CurrentHP <= 0.0f) {
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
		if (CurrentHP <= 0.0f) {
			//���⿡ ĳ���� ����Լ� ���ߵ˴ϴ�
			CurrentHP = 0.0f;
			DeathAnim();
		}
	}
}

void ACh_SpiderBase::DeathAnim_Implementation()
{
	if(IsValid(DeathAnimAsset))
	{
		GetMesh()->PlayAnimation(DeathAnimAsset, false);
	}
}