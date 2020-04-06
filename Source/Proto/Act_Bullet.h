// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
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

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	//나중에 ApplyDamage할 때 줄 컨트롤러 값(플레이어 또는 AI)
	AController* DamageInstigatorPlayer = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	USceneComponent* DefaultSceneRoot;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	class UStaticMeshComponent* BulletMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	class UCapsuleComponent* BulletCollision;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet|Tracer")
	class UParticleSystemComponent* TracerFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet")
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Bullet")
	float Damage;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet|Tracer", meta = (AllowPrivateAccess = true))
	FLinearColor TracerColor = FLinearColor(0.87f, 0.03f, 0.0f, 0.5f);

public:
	UFUNCTION(BlueprintCallable)
	/**	총알의 속도, 데미지, 색 등을 초기화 하고 발동시킵니다.
	*	총알은 Spawn될 때 AActor* FActorSpawnParameters.Owner를 통해 자신을 발사한 무기의 정보를 얻습니다.
	*	그리고 이 Initiallize에서 무기를 통해 WCS와 무기가 장착된 Actor, 그 무기의 Controller를 구합니다.
	*	여기서 
	*/
	void InitializeBullet(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor = FLinearColor(0.87f,0.03f,0.0f,0.5f));

	UFUNCTION( )
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex, 
                      bool bFromSweep, 
                      const FHitResult &SweepResult );
};
