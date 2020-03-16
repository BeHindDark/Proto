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
		동작 순서
		1. HttpRequest(요청) 생성한다
		2. 요청 방식은 "Get" or "Post" 방식으로 설정한다
		3. 요청 URL은 인자값으로 받은 URL을 사용하도록 설정한다
		4. *중요, 요청 후 프레임마다 받은 바이트의 크기를 리턴해준다
		해당 OnAccountCreationResponse 함수를 연결시켜준다
		5. 요청이 완료됬을 때 호출할 함수를 연결해준다
		6. 요청을 보내준다
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

	//웹서버로부터 응답된 내용을 화면에 디버그 문자열로 출력해본다
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
	//웹서버로부터 응답된 내용을 화면에 디버그 문자열로 출력해본다
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

