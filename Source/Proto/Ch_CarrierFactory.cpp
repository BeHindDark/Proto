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
	
	//기본값(컴포넌트 등록, 좌표, 기타 기본 값) 정렬
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	WCS = CreateDefaultSubobject<UWeaponControlSystem>(TEXT("WeaponControlSystem"));

	CameraPitchSpeed = 60.0f;
	CameraYawSpeed = 120.0f;
	
	SpringArm->TargetArmLength = 800.0f;
	SpringArm->SocketOffset = FVector(0.0f, 0.0f, 300.0f);

	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera->SetupAttachment(SpringArm);

	SpringArm->bInheritPitch = false;
	SpringArm->bInheritYaw = false;
	SpringArm->bInheritRoll = false;
	Camera->bUsePawnControlRotation = false;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 30.0f, 0.0f);

	//스태틱 메시에 붙어있는 각 소켓의 이름 정렬
	FName TopSocket(TEXT("Mount_Top"));
	FName CockpitSocket(TEXT("Mount_Cockpit"));
	FName LShoulderSocket(TEXT("Mount_HalfShoulder_L"));
	FName RShoulderSocket(TEXT("Mount_HalfShoulder_R"));
	FName LWeaponSocket(TEXT("Mount_Weapon_L"));
	FName RWeaponSocket(TEXT("Mount_Weapon_R"));
	FName WeaponSocket(TEXT("Mount_Weapon"));

	FName TopComponentName(TEXT("ShoulderMesh"));
	
	const USkeletalMeshSocket* MainMeshSocket = GetMesh()->GetSocketByName(TopSocket);
	
	//메시에 소켓이름이 없음에도 , 메시 자체가 지정되지 않았음에도 컴포넌트가 attach되는 것을 확인, 메시가 생성되면 해당 소켓으로 붙는것도 같이 확인.
	//if (GetMesh()->DoesSocketExist(TopSocket)) {
	//애로우를 이용해서 간접적으로 붙이지말고 그냥 소켓에 직접적으로 붙이기
	//CockpitArrow = CreateDefaultSubobject<UArrowComponent>(TEXT("CockpitArrow"));
		ShoulderMesh = CreateDefaultSubobject<UStaticMeshComponent>(TopComponentName);
		ConstructorHelpers::FObjectFinder<UStaticMesh> SHOULDER(TEXT("/Game/Mech_Constructor_Spiders/Meshes/Shoulders_Med_Tank.Shoulders_Med_Tank"));
		if (SHOULDER.Succeeded()) {
			ShoulderMesh->SetStaticMesh(SHOULDER.Object);
		}

		ShoulderMesh->SetupAttachment(GetMesh(), TopSocket);
	//}
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

// Called every frame
void ACh_CarrierFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Camera != nullptr) {
		Camera->AddRelativeRotation(FRotator(CameraPitchMovement * DeltaTime, 0.0f, 0.0f));
	}
	if (Camera != nullptr) {
		SpringArm->AddRelativeRotation(FRotator(0.0f, CameraYawMovement * DeltaTime, 0.0f));
	}

	if (bIsPlayerControlling) {
		AimLocation = CameraAimLocation(Camera);
	}

	WCS->TargetWorldLocation = AimLocation;

	//TurnShoulderMesh(ShoulderMesh, TEXT("Mount_Top"), DeltaTime);
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

void ACh_CarrierFactory::TurnBody(float NewAxisValue) {
	AddControllerYawInput(NewAxisValue * BodyYawSpeed);
}

void ACh_CarrierFactory::TurnShoulderMesh(UStaticMeshComponent* ShoulderComponent, FName SocketName, float DeltaTime) {
	FRotator TargetDirection = FTransform(FRotator::ZeroRotator, AimLocation).GetRelativeTransform(GetActorTransform()).GetLocation().ToOrientationRotator();
	FRotator TurnRotation = FRotator(0.0f, TargetDirection.Yaw, 0.0f);
	FRotator CurrentShoulderRotation = ShoulderComponent->GetRelativeTransform().Rotator();
	FRotator TurningRotator = FRotator::ZeroRotator;
	FRotator TempRotator = FRotator::ZeroRotator;

	(TargetDirection - TurnRotation).GetWindingAndRemainder(TempRotator, TurningRotator);

	if (!TurningRotator.IsNearlyZero(0.001f)) {
		FRotator DeltaRotation = FRotator::ZeroRotator;
		if (FMath::Abs(TurningRotator.Yaw) <= ShoulderMeshRotationSpeed * DeltaTime) {
			DeltaRotation.Yaw = TurningRotator.Yaw;
		}
		else {
			DeltaRotation.Yaw = FMath::Sign(TurningRotator.Yaw) * ShoulderMeshRotationSpeed * DeltaTime;
		}

		FRotator NewShoulderRotation = CurrentShoulderRotation + DeltaRotation;
		NewShoulderRotation.Pitch = 0.0f;
		NewShoulderRotation.Roll = 0.0f;
		ShoulderComponent->SetRelativeRotation(NewShoulderRotation);
	}
}