// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginHttp.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"

// Sets default values
ALoginHttp::ALoginHttp()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	Http = &FHttpModule::Get();

}


void ALoginHttp::SendAccountCreationRequest(const FString& userId, const FString& userPw)
{
	// Http�� ���� ��û
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	// http�� ��û�� �����Ѵ�. 
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginHttp::OnAccountCreationResponse);
	// http�� userId, userPw �����͸� url�� �̿��Ͽ� �����͸� �����ش�
	Request->SetURL(FString::Printf(TEXT("http://192.168.116.135::80/d.php?userId=%s&userPw=%s"), *userId, *userPw));
	// Get���� ��û�� ���ش�
	Request->SetVerb("Get");
	Request->SetHeader(TEXT("User-Agent"), "x-UnrealEngin-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	// ��û ó���� �����ϱ� ���� ȣ��
	Request->ProcessRequest();
}

void ALoginHttp::OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;

	//TSharedRef<<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	/*
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		bool bAccountCreatedSuccessfully = JsonObject->GetBoolField("AccountSuccessfullyCreated");
		FString Username = JsonObject->GetStringField("Username");
	}
	*/
}

// Called when the game starts or when spawned
void ALoginHttp::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALoginHttp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

