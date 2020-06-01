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
	
	//기본값(컴포넌트 등록, 좌표, 기타 기본 값) 정렬
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

/**	변수 리플리케이션을 할 때 항상 삽입해야 하는 함수입니다.
*	선언은 없이 정의부분만 넣어주면 됩니다.
*	빨간줄은 무시하세요
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
	
	//DeathAnimAssset이 재생 끝나는걸 알리는 notify에 웨폰 및 본체를 파괴시키는 로직을 추가하자.
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
			//카메라 회전
			Camera->AddRelativeRotation(FRotator(CameraPitchMovement * DeltaTime,0.0f,0.0f));
			SpringArm->AddRelativeRotation(FRotator(0.0f,CameraYawMovement * DeltaTime,0.0f));
		}
	}

	if(IsValid(WaistSceneComponent))
	{
		TurnUpperBody(WaistSceneComponent,DeltaTime);
	}

	//DeltaTime은 약 0.003~0.005입니다.
	TickTimeStack += DeltaTime;

	if(TickTimeStack>=NetworkTickInterval)
	{
		//네트워크 함수 실행
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


	//무기 선택 액션 바인딩
	//바인드할 입력 이름 설정
	TArray<FName> WeaponGroupName ={FName(TEXT("WeaponGroup1")), FName(TEXT("WeaponGroup2")), FName(TEXT("WeaponGroup3"))};
	for(int i=0; i<3; i++)
	{
		//바이드할 액션 이름과 키로 액션바인딩 생성
		FInputActionBinding WeaponGroupActionBinding(WeaponGroupName[i],IE_Pressed);

		//델리게이트 생성
		FInputActionHandlerSignature WeaponGroupActionHandler;

		//델리게이트에 함수 바인딩
		WeaponGroupActionHandler.BindUFunction(this,FName("ChangeWeaponGroup"),i);

		//액션바인딩에 델리게이트 설정
		WeaponGroupActionBinding.ActionDelegate = WeaponGroupActionHandler;

		//완성된 액션바인딩을 InputComponent에 추가
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
	//Actors to ignore을 spider의 변수로 만들고, 리플리케이트 되게 한다.
	//또한 Weapon을 attach시킬 때 ignore에 추가시키는 로직을 만든다.(WCS에서)
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
	FRotator TargetUpperBodyRotation = FRotator(0.0f,RelativeTargetDirection.Yaw,0.0f);

	//현재 상체의 상대회전
	FRotator CurrentUpperBodyRotation = WaistComponent->GetRelativeTransform().Rotator();

	//현재 회전과 목표 회전의 차이값
	FRotator RotationDiff = FRotator::ZeroRotator;

	//덜어내기용 더미값
	FRotator DummyRot = FRotator::ZeroRotator;

	//이 함수를 이용해 Rotation을 Pitch, yaw, Roll이 -180 ~ +180의 값을 갖도록 한다.
	(TargetUpperBodyRotation - CurrentUpperBodyRotation).GetWindingAndRemainder(DummyRot,RotationDiff);

	//회전각 차이가 너무 작으면 돌리지 않는다.
	if(!RotationDiff.IsNearlyZero(0.001f))
	{
		//이번 틱에 돌려야하는 로테이션
		FRotator DeltaRotation = FRotator::ZeroRotator;
		if(FMath::Abs(RotationDiff.Yaw) <= UpperBodyRotationSpeed * DeltaTime)
		{
			DeltaRotation.Yaw = RotationDiff.Yaw;
		}
		else
		{
			DeltaRotation.Yaw = FMath::Sign(RotationDiff.Yaw) * UpperBodyRotationSpeed * DeltaTime;
		}

		//새로운 로테이션의 최종값, 찌꺼기값이 쌓이는 걸 방지하기 위해 pich와 roll은 0으로 고정된다.
		FRotator NewUpperBodyRotation = CurrentUpperBodyRotation + DeltaRotation;
		
		//적용
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
		//정말로 간단한 체력계산식
		CurrentHP -= Damage;
		//위젯에 체력 넘겨주는 부분
		//열거형 사용해서 어떤 부위의 HP인지 매개변수로 넘겨줌
		/*
		if(IsValid(WG_InGame))
		{
			WG_InGame->SetHP(HPType::BODY,(int)CurrentHP);
		}
		*/
		if (CurrentHP <= 0.0f) {
			OnDeath.Broadcast(EventInstigator);
			//여기에 캐릭터 사망함수 혹은 패배관련 함수가 들어가야됩니다
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
			//여기에 캐릭터 사망함수 들어가야됩니다
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