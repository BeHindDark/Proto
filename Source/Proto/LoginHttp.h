// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "GameFramework/Actor.h"
#include "Online/HTTP/Public/Http.h"
#include "LoginHttp.generated.h"

UCLASS()
class PROTO_API ALoginHttp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALoginHttp();

	FHttpModule* Http;

	// http�� url�� ��û Get���� userId, userPw�� �������ִ� �Լ� 
	UFUNCTION(BlueprintCallable, Category = "LoginHttp")
	void SendAccountCreationRequest(const FString& userId, const FString& userPw);

	// http Get ��û ���� ȣ��Ǵ� �ݹ��Լ�
	void OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
