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
	//���߿� ApplyDamage�� �� �� ��Ʈ�ѷ� ��(�÷��̾� �Ǵ� AI)
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet")
	FVector ProjectileVelocity;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet|Tracer", meta = (AllowPrivateAccess = true))
	FLinearColor TracerColor = FLinearColor(0.87f, 0.03f, 0.0f, 0.5f);
	
	UFUNCTION()
	void HitCheck(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

public:
	/**	�Ѿ��� �ӵ�, ������, �� ���� �ʱ�ȭ �ϰ� �ߵ���ŵ�ϴ�.
	*	�Ѿ��� Spawn�� �� AActor* FActorSpawnParameters.Owner�� ���� �ڽ��� �߻��� ������ ������ ����ϴ�.
	*	�׸��� �� Initiallize���� ���⸦ ���� WCS�� ���Ⱑ ������ Actor, �� ������ Controller�� ���մϴ�.
	*	���⼭ 
	*/
	UFUNCTION(BlueprintCallable, NetMulticast, Reliable, WithValidation)
	void InitializeBullet(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor = FLinearColor(0.87f,0.03f,0.0f,0.5f));
	bool InitializeBullet_Validate(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor = FLinearColor(0.87f, 0.03f, 0.0f, 0.5f));
	void InitializeBullet_Implementation(float InitialSpeed, float WeaponDamage, FLinearColor NewTracerColor = FLinearColor(0.87f, 0.03f, 0.0f, 0.5f));

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void StopFX();
	bool StopFX_Validate();
	void StopFX_Implementation();


	UFUNCTION( )
    void BeginOverlap(UPrimitiveComponent* OverlappedComponent, 
                      AActor* OtherActor, 
                      UPrimitiveComponent* OtherComp, 
                      int32 OtherBodyIndex, 
                      bool bFromSweep, 
                      const FHitResult &SweepResult );
};
