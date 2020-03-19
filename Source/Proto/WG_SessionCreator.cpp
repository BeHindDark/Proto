// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionCreator.h"
#include "WG_SessionInfo.h"
#include "WG_SessionBrowser.h"
#include "GI_Proto.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/MultiLineEditableText.h"

UWG_SessionCreator::UWG_SessionCreator(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> UMG_LoadingScreen_C(TEXT("/Game/Blueprints/Widget/UMG_LoadingScreen.UMG_LoadingScreen_C"));
	if(UMG_LoadingScreen_C.Succeeded()) {
		LoadingScreen_Class = UMG_LoadingScreen_C.Class;
	}
}

void UWG_SessionCreator::NativeConstruct()
{
	Super::NativeConstruct();

	CreateSessionButton->OnClicked.AddDynamic(this,&UWG_SessionCreator::OnCreateSessionClicked);
	ClearButton->OnClicked.AddDynamic(this,&UWG_SessionCreator::OnClearClicked);
	BacktoBrowserButton->OnClicked.AddDynamic(this,&UWG_SessionCreator::OnBacktoBrowserClicked);
}

void UWG_SessionCreator::SetUpperClassWidget(UWG_SessionBrowser * SessionBrowser)
{
	if(IsValid(SessionBrowser))
	{
		WG_SessionBrowser_Ref = SessionBrowser;
	}

}

void UWG_SessionCreator::OnGetCreateSessionReport(bool bWasSuccessful)
{
	UGI_Proto* ProtoGI = Cast<UGI_Proto>(GetGameInstance());
	if(!IsValid(ProtoGI))
	{
		CHECK_LOG(!IsValid(ProtoGI));
		return;
	}

	ProtoGI->OnCreateSessionReport.Clear();

	if(!bWasSuccessful)
	{
		//error
		//이부분은 PlayerController로 옮겨야 한다.
		if(IsValid(LoadingScreen))
		{
			LoadingScreen->RemoveFromParent();
			LoadingScreen = nullptr;
		}

	}
	else
	{
		UGameplayStatics::OpenLevel(GetWorld(),FName("ThirdPersonExampleMap"),true,"listen");
	}


}

void UWG_SessionCreator::OnCreateSessionClicked()
{
	UGI_Proto* ProtoGI = Cast<UGI_Proto>(GetGameInstance());
	if(!IsValid(ProtoGI))
	{
		CHECK_LOG(!IsValid(ProtoGI));
		return;
	}

	ProtoGI->OnCreateSessionReport.AddUObject(this,&UWG_SessionCreator::OnGetCreateSessionReport);

	ProtoGI->CreateOnlineSession(WG_SessionInfo->SessionNameTextBox->GetText().ToString(),
								 WG_SessionInfo->HostNameTextBox->GetText().ToString(),
								 WG_SessionInfo->DescriptionMLEditableText->GetText().ToString(),
								 4);
	//아래의 로딩스크린 부르는 부분은 Player Controller로 옮길 예정
	LoadingScreen = CreateWidget<UUserWidget>(GetOwningPlayer(),LoadingScreen_Class);
	LoadingScreen->AddToViewport(10);
}

void UWG_SessionCreator::OnClearClicked()
{
	WG_SessionInfo->ClearText();
}

void UWG_SessionCreator::OnBacktoBrowserClicked()
{
	if(WG_SessionBrowser_Ref==nullptr)
	{
		//error
		CHECK_LOG(WG_SessionBrowser_Ref==nullptr);
	}
	else
	{
		WG_SessionBrowser_Ref->SetVisibility(ESlateVisibility::Visible);
		RemoveFromParent();
	}

}
