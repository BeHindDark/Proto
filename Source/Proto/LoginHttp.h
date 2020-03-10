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

	// http로 url로 요청 Get으로 userId, userPw로 전달해주는 함수 
	UFUNCTION(BlueprintCallable, Category = "LoginHttp")
	void SendAccountCreationRequest(const FString& userId, const FString& userPw);

	// http Get 요청 직후 호출되는 콜백함수
	void OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
