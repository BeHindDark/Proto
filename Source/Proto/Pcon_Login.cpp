// Fill out your copyright notice in the Description page of Project Settings.


#include "Pcon_Login.h"
#include "WG_Login.h"

APcon_Login::APcon_Login()
{

	static ConstructorHelpers::FClassFinder<UWG_Login> WG_Login_C(TEXT("/Game/Blueprints/Widget/WG_Login.WG_Login_C"));
	if(WG_Login_C.Succeeded())
	{
		WG_Login_Class = WG_Login_C.Class;
	}

}

void APcon_Login::BeginPlay()
{
	Super::BeginPlay();

	WG_Login_Ref = CreateWidget<UWG_Login>(this,WG_Login_Class);
}