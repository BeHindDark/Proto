// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Act_Bullet.generated.h"

UCLASS()
class PROTO_API AAct_Bullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAct_Bullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet|Tracer")
	class UParticleSystemComponent* TracerFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	class UCapsuleComponent* CollisionCapsule;
		
};
