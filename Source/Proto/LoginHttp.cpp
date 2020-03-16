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


void ALoginHttp::SendAccountCreationRequest(const FString& userId, const FString& userPw, const FString& userName)
{
	/*
		���� ����
		1. HttpRequest(��û) �����Ѵ�
		2. ��û ����� "Get" or "Post" ������� �����Ѵ�
		3. ��û URL�� ���ڰ����� ���� URL�� ����ϵ��� �����Ѵ�
		4. *�߿�, ��û �� �����Ӹ��� ���� ����Ʈ�� ũ�⸦ �������ش�
		�ش� OnAccountCreationResponse �Լ��� ��������ش�
		5. ��û�� �Ϸ���� �� ȣ���� �Լ��� �������ش�
		6. ��û�� �����ش�
	*/
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("userName : %s, Id : %s, Pw : %s"), *userName, *userId, *userPw));

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetVerb("Get");
	Request->SetURL(FString::Printf(TEXT("http://192.168.161.142:8080/join.php?userId=%s&userPw=%s&userName=%s"), *userId, *userPw, *userName));
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginHttp::OnAccountCreationResponse);
	Request->SetHeader(TEXT("User-Agent"), "x-UnrealEngin-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void ALoginHttp::OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{

	//�������κ��� ����� ������ ȭ�鿡 ����� ���ڿ��� ����غ���
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, Response->GetContentAsString());

	/*
	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		bool bAccountCreatedSuccessfully = JsonObject->GetBoolField("AccountSuccessfullyCreated");
		FString userId = JsonObject->GetStringField("userId");

		//GEngine->AddOnScreenDebugMessage(1, 30.0f, FColor::Blue, FString::Printf(TEXT("%s"), userId));
	}
	*/
}

void ALoginHttp::SendLoginRequest(const FString& userId, const FString& userPw)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("userName : Id : %s, Pw : %s"), *userId, *userPw));

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetVerb("Get");
	Request->SetURL(FString::Printf(TEXT("http://192.168.161.142:8080/login.php?userId=%s&userPw=%s&userName=%s"), *userId, *userPw));
	Request->OnProcessRequestComplete().BindUObject(this, &ALoginHttp::OnLoginResponse);
	Request->SetHeader(TEXT("User-Agent"), "x-UnrealEngin-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void ALoginHttp::OnLoginResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//�������κ��� ����� ������ ȭ�鿡 ����� ���ڿ��� ����غ���
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, Response->GetContentAsString());
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

