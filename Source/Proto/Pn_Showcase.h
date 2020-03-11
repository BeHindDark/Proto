// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/Pawn.h"
#include "Pn_Showcase.generated.h"

UCLASS()
class PROTO_API APn_Showcase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APn_Showcase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, Category = Root)
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* Camera;
};
