// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Login.h"
#include "WG_Login.h"

APC_Login::APC_Login() {
	static ConstructorHelpers::FClassFinder<UWG_Login> WG_Login_C(TEXT("/Game/Blueprints/Widget/UMG_Login.UMG_Login_C"));
	if (WG_Login_C.Succeeded()) {
		WG_Login_Class = WG_Login_C.Class;
	}
}

void APC_Login::BeginPlay() {
	Super::BeginPlay();

	WG_Login_Ref = CreateWidget<UWG_Login>(this, WG_Login_Class);
	WG_Login_Ref->AddToViewport(0);

	bShowMouseCursor = true;
}

void APC_Login::OnLogin(FText ID, FText PW) {

}