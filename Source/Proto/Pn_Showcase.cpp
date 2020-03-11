// Fill out your copyright notice in the Description page of Project Settings.


#include "Pn_Showcase.h"

// Sets default values
APn_Showcase::APn_Showcase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneRoot"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	RootComponent = DefaultSceneRoot;
	Camera->SetupAttachment(DefaultSceneRoot);

}

// Called when the game starts or when spawned
void APn_Showcase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APn_Showcase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APn_Showcase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

