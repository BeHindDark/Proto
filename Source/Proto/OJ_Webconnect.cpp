// Fill out your copyright notice in the Description page of Project Settings.


#include "OJ_Webconnect.h"
#include "Json/Public/Serialization/JsonReader.h"
#include "Json/Public/Serialization/JsonSerializer.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SocketSubsystem.h"



void UOJ_Webconnect::PostInitProperties()
{
	Super::PostInitProperties();

	Http = &FHttpModule::Get();

}

const FString& UOJ_Webconnect::GetUserId() const
{
	// TODO: ���⿡ return ���� �����մϴ�.
	return s_UserId;
}

void UOJ_Webconnect::SendAccountCreationRequest(const FString& userId, const FString& userPw, const FString& userPW2)
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

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetVerb("Get");
	Request->SetURL(FString::Printf(TEXT("http://192.168.116.144:8080/join.php?userId=%s&userPw=%s&userName=%s"), *userId, *userPw, *userPW2));
	Request->OnProcessRequestComplete().BindUObject(this, &UOJ_Webconnect::OnAccountCreationResponse);
	Request->SetHeader(TEXT("User-Agent"), "x-UnrealEngin-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();
}

void UOJ_Webconnect::OnAccountCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	//�������κ��� ����� ������ ȭ�鿡 ����� ���ڿ��� ����غ���
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, Response->GetContentAsString());

	TSharedPtr<FJsonObject> JsonObject;

	//Create a reader pointer to read the json data
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{

	}
}

// �α��� ��û
void UOJ_Webconnect::SendLoginCreationRequest(const FString& userId, const FString& userPw)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("userName : Id : %s, Pw : %s"), *userId, *userPw));

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->SetVerb("Get");
	Request->SetURL(FString::Printf(TEXT("http://192.168.116.147:8080/login.php?userId=%s&userPw=%s"), *userId, *userPw));
	Request->OnProcessRequestComplete().BindUObject(this, &UOJ_Webconnect::OnLoginCreationResponse);
	Request->SetHeader(TEXT("User-Agent"), "x-UnrealEngin-Agent");
	Request->SetHeader("Content-Type", TEXT("application/json"));
	Request->ProcessRequest();

	//OnLoginResponse = LoginResponse;

}

// �α��� ����
void UOJ_Webconnect::OnLoginCreationResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	bool bSuccess = false;
	FString Result;

	//�������κ��� ����� ������ ȭ�鿡 ����� ���ڿ��� ����غ���
	GEngine->AddOnScreenDebugMessage(1, 10.0f, FColor::Green, Response->GetContentAsString());

	//json ����ȭ �����͸� ������ ������ ����
	TSharedPtr<FJsonObject> JsonObject;

	//json �����͸� ���� �ǵ��� ������ ����
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	//�����ڿ��� �־��� json �����Ϳ� deserialize�� ���� ��ü ����ȭ
	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		bool bLoginWasSuccessful = JsonObject->GetBoolField("LoginWasSuccessful");
		// �α��� �����ϸ� 
		if (bLoginWasSuccessful)
		{
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Green, TEXT("Login true"));
			
			s_UserId = JsonObject->GetStringField("ID");
			
			bSuccess = true;
			Result = TEXT("�α��� ����");

			// MainMap���� �̵��Ѵ�
			UGameplayStatics::OpenLevel(this, "MainMap");
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Green, TEXT("Login false"));
		}
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Login false"));
	}

	//OnLoginResponse.Execute(bSuccess, Result);
}
