// Fill out your copyright notice in the Description page of Project Settings.


#include "Ch_CarrierFactory.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "WeaponControlSystem.h"
#include "Act_WeaponBase.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
ACh_CarrierFactory::ACh_CarrierFactory()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	//기본값(컴포넌트 등록, 좌표, 기타 기본 값) 정렬
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

	
	//OnTakeAnyDamage.AddDynamic(this, &ACh_CarrierFactory::함수);
}

void ACh_CarrierFactory::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	//변수 리플리케이션을 할 때 항상 삽입해야 하는 함수입니다.
	//선언은 없이 정의부분만 넣어주면 됩니다.
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACh_CarrierFactory, AimLocation);
}

// Called when the game starts or when spawned
void ACh_CarrierFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACh_CarrierFactory::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	if (IsPlayerControlled()) {
		PlayerController = Cast<APlayerController>(GetController());
		if (PlayerController != nullptr) {
			bIsPlayerControlling = true;
		}
	}
	
}

void ACh_CarrierFactory::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	WCS->InitializeWeaponNumber(WeaponSlotNum);
}

// Called every frame
void ACh_CarrierFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsPlayerControlling)
	{
		if(GetController()->IsLocalPlayerController())
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
			ServerNetTick(NewLocalAim, DeltaTime);
		}
	}
	

	
	
}

// Called to bind functionality to input
void ACh_CarrierFactory::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ACh_CarrierFactory::Turn);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ACh_CarrierFactory::LookUp);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACh_CarrierFactory::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACh_CarrierFactory::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("TurnBody"), this, &ACh_CarrierFactory::TurnBody);

}

FVector ACh_CarrierFactory::CameraAimLocation(UCameraComponent* CurrentCamera) {
	FVector AimPoint = Camera->GetComponentLocation() + (Camera->GetForwardVector() * 10000.0f);
	FCollisionQueryParams AimParams;
	FHitResult AimResult;
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	for (FWeaponData WeaponData:WCS->WeaponDataArray) {
		ActorsToIgnore.Add(WeaponData.Weapon);
	}
	AimParams.AddIgnoredActors(ActorsToIgnore);
	bool IsHit = GetWorld()->LineTraceSingleByChannel(AimResult, Camera->GetComponentLocation(), AimPoint, ECC_Visibility, AimParams);
	if (IsHit) {
		if (AimResult.bBlockingHit) {
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

void ACh_CarrierFactory::Turn(float NewAxisValue) {
	CameraYawMovement = CameraYawSpeed * NewAxisValue;
}

void ACh_CarrierFactory::LookUp(float NewAxisValue) {
	CameraPitchMovement = CameraPitchSpeed * -NewAxisValue;
}

void ACh_CarrierFactory::MoveForward(float NewAxisValue) {
	MoveInput = NewAxisValue;
	AddMovementInput(GetActorForwardVector(), NewAxisValue);
}

void ACh_CarrierFactory::MoveRight(float NewAxisValue) {
	if (FMath::IsNearlyEqual(MoveInput, 0.0f, 0.01f))
	{
		AddMovementInput(GetActorRightVector(), NewAxisValue);
	}
	else
	{
		if (MoveInput > 0.0f)
		{
			AddControllerYawInput(NewAxisValue * BodyYawSpeed);
		}
		else
		{
			AddControllerYawInput(-NewAxisValue * BodyYawSpeed);
		}
	}
}

void ACh_CarrierFactory::TurnBody(float NewAxisValue)
{
	AddControllerYawInput(NewAxisValue * BodyYawSpeed);
}

void ACh_CarrierFactory::SetWaistSceneComponent(USceneComponent * BlueprintWaistSceneComponent)
{
	WaistSceneComponent = BlueprintWaistSceneComponent;
}


void ACh_CarrierFactory::ServerNetTick_Implementation(FVector CameraAim, float Deltatime)
{
	if(GetLocalRole()<ROLE_Authority)
	{
		return;
	}
	AimLocation = CameraAim;
	WCS->TargetWorldLocation = AimLocation;
	if(IsValid(WaistSceneComponent))
	{
		TurnUpperBody(WaistSceneComponent,Deltatime);
	}
}

bool ACh_CarrierFactory::ServerNetTick_Validate(FVector CameraAim,float Deltatime)
{
	return true;
}

void ACh_CarrierFactory::TurnUpperBody(USceneComponent* WaistComponent, float DeltaTime)
{
	//상대좌표계 상에서 목표의 방향
	FRotator RelativeTargetDirection;
	
	//만약 제대로 소켓에 붙어있다면 소켓기준 좌표를, 아니면 액터 기준 좌표계를 이용
	if(!WaistComponent->GetAttachSocketName().IsNone())
	{
		const USkeletalMeshSocket* WaistSocket = GetMesh()->GetSocketByName(WaistComponent->GetAttachSocketName());
		RelativeTargetDirection = FTransform(FRotator::ZeroRotator,AimLocation).GetRelativeTransform(WaistSocket->GetSocketTransform(GetMesh())).GetLocation().ToOrientationRotator();
	}
	else
	{
		RelativeTargetDirection = FTransform(FRotator::ZeroRotator,AimLocation).GetRelativeTransform(GetActorTransform()).GetLocation().ToOrientationRotator();
	}
	
	//포탑이 가져야하는 상대회전
	FRotator TargetUpperBodyRotation = FRotator(0.0f,RelativeTargetDirection.Yaw, 0.0f);

	//현재 상체의 상대회전
	FRotator CurrentUpperBodyRotation = WaistComponent->GetRelativeTransform().Rotator();

	//현재 회전과 목표 회전의 차이값
	FRotator RotationDiff = FRotator::ZeroRotator;

	//덜어내기용 더미값
	FRotator DummyRot = FRotator::ZeroRotator;

	//이 함수를 이용해 Rotation을 Pitch, yaw, Roll이 -180 ~ +180의 값을 갖도록 한다.
	(TargetUpperBodyRotation - CurrentUpperBodyRotation).GetWindingAndRemainder(DummyRot, RotationDiff);

	//회전각 차이가 너무 작으면 돌리지 않는다.
	if (!RotationDiff.IsNearlyZero(0.001f))
	{
		//이번 틱에 돌려야하는 로테이션
		FRotator DeltaRotation = FRotator::ZeroRotator;
		if (FMath::Abs(RotationDiff.Yaw) <= UpperBodyRotationSpeed * DeltaTime)
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