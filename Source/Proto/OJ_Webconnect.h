// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "UObject/NoExportTypes.h"

#include "Online/HTTP/Public/Http.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include "OJ_Webconnect.generated.h"

DECLARE_DELEGATE_TwoParams(FOnLoginResponse, bool, const FString&);

/**
 * 
 */
UCLASS()
class PROTO_API UOJ_Webconnect : public UObject
{
	GENERATED_BODY()

		
public:
	virtual void PostInitProperties() override;

	FOnLoginResponse OnLoginResponse;

private:
    FHttpModule* Http;

	FString s_UserId;
public:

	const FString& GetUserId() const;

public:
	
	// http로 url로 요청 Get으로 userId, userPw로 전달해주는 함수 
	//UFUNCTION(BlueprintCallable, Category = "LoginHttp")
	void SendAccountCreationRequest(const FString& userId, const FString& userPw, const FString& userPW2);

	// http Get 요청 직후 호출되는 콜백함수
	void OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
	//UFUNCTION(BlueprintCallable, Category = "LoginHttp")
	void SendLoginCreationRequest(const FString& userId, const FString& userPw);
	void OnLoginCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	
};
