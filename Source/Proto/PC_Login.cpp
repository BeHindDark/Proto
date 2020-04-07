// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Login.h"
#include "WG_Login.h"
#include "WG_Join.h"
#include "LoginHttp.h"
#include "GI_Proto.h"
#include "Blueprint/UserWidget.h"
#include "Json.h"

APC_Login::APC_Login() 
{
	InitializeWidget();
}

void APC_Login::BeginPlay() {
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (WG_Login_Class)
		{
			WG_Login_Ref = CreateWidget<UWG_Login>(this, WG_Login_Class);

			FInputModeUIOnly Mode;
			Mode.SetWidgetToFocus(WG_Login_Ref->GetCachedWidget());
			SetInputMode(Mode);
			WG_Login_Ref->AddToViewport(0);
			bShowMouseCursor = true;
		}
	}

}


void APC_Login::OnLogin(FText ID, FText PW)
{
	//RequestLogin() 함수 일단 봉인 
	RequestLogin(ID.ToString(), PW.ToString());
	/*
	FActorSpawnParameters spawnparams;
	spawnparams.Owner = this;
	ALoginHttp* Login_Http = GetWorld()->SpawnActor<ALoginHttp>(FVector::ZeroVector, FRotator::ZeroRotator, spawnparams);
	Login_Http->SendLoginRequest(ID.ToString(), PW.ToString());
	*/
}

void APC_Login::OnJoin(FText ID, FText PW, FText PW2)
{
	
	FActorSpawnParameters spawnparams;
	spawnparams.Owner = this;
	ALoginHttp* Login_Http = GetWorld()->SpawnActor<ALoginHttp>(FVector::ZeroVector, FRotator::ZeroRotator, spawnparams);
	Login_Http->SendAccountCreationRequest(ID.ToString(), PW.ToString(), PW2.ToString());
	
}

void APC_Login::ShowJoinWG(int Zorder)
{
	
	if (IsValid(WG_Join))
	{
		WG_Join->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		WG_Join = nullptr;
		WG_Join = CreateWidget<UWG_Join>(this, WG_Join_Class);
	}

	if (!WG_Join->IsInViewport())
	{
		WG_Join->AddToViewport(Zorder);
	}
	
	bShowMouseCursor = true;
	
}

void APC_Login::InitializeWidget()
{
	static ConstructorHelpers::FClassFinder<UWG_Login> WG_Login_C(TEXT("/Game/Blueprints/Widget/UMG_Login.UMG_Login_C"));
	if (WG_Login_C.Succeeded()) {
		WG_Login_Class = WG_Login_C.Class;
	}
	static ConstructorHelpers::FClassFinder<UWG_Join> WG_Join_C(TEXT("/Game/Blueprints/Widget/UMG_Join.UMG_Join_C"));
	if (WG_Join_C.Succeeded())
	{
		WG_Join_Class = WG_Join_C.Class;
	}

}

// 일단 봉인 나중에 GetGameInstance 분석하자
void APC_Login::RequestLogin(const FString& UserID, const FString& UserPW)
{

	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("userName : Id : %s, Pw : %s"), *UserID, *UserPW));
	
	auto MyGameInstance = GetGameInstance<UGI_Proto>();
	if (nullptr == MyGameInstance)
	{
		UE_LOG(Proto, Error, TEXT("Invalid game instance.."));
		return;
	}
	
	//FInputActionHandlerSignature::CreateUObject(this, &APC_Login::ReceiveLoginResponse);

	//FOnLoginResponse::CreateUObject(this,&APC_Login::ReceiveLoginResponse);
	
	MyGameInstance->GetWebconnect().SendLoginCreationRequest(UserID, UserPW);
	
	/*
	MyGameInstance->GetWebConnector().Login(UserID, UserPW,
		FOnLoginResponse::CreateUObject(this, &UZInputIDWidget::ReceiveLoginResponse));
		*/
}

void APC_Login::ReceiveLoginResponse()
{
	// MainMap으로 이동한다
	UGameplayStatics::OpenLevel(this, "MainMap");
}



