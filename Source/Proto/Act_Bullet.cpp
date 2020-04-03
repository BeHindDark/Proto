// Fill out your copyright notice in the Description page of Project Settings.


#include "Act_Bullet.h"

// Sets default values
AAct_Bullet::AAct_Bullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
}

// Called when the game starts or when spawned
void AAct_Bullet::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAct_Bullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

