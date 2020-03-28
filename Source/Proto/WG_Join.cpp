// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Join.h"
#include "PC_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"

void UWG_Join::NativeConstruct()
{
	Super::NativeConstruct();
	
	IDInput->OnTextCommitted.AddDynamic(this, &UWG_Join::IDCommitted);
	PWInput->OnTextCommitted.AddDynamic(this, &UWG_Join::PassWordCommitted);
	PW2Input->OnTextCommitted.AddDynamic(this, &UWG_Join::PassWordCommitted);
	JoinButton->OnClicked.AddDynamic(this, &UWG_Join::OnJoinClicked);
	
}

void UWG_Join::IDCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		PWInput->SetKeyboardFocus();
	}
}

void UWG_Join::PassWordCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		PW2Input->SetKeyboardFocus();
	}
}

void UWG_Join::PassWord2Committed(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter) {
		UWG_Join::OnJoinSession();
	}
}

void UWG_Join::OnJoinClicked()
{
	UWG_Join::OnJoinSession();
}

void UWG_Join::OnJoinSession()
{
	/*
	//PW있어야됨
	if (PWInput->GetText().ToString().Len() != 0) 
	{
		if (PWInput->GetText().ToString() == PW2Input->GetText().ToString())
		{
			//ID길이 5자 이상 제한
			if (IDInput->GetText().ToString().Len() >= 5) 
			{
				APC_Login* PC = Cast<APC_Login>(GetOwningPlayer());
				//ID 5자 이상, PW 존재라는 조건을 통과해야 OnLogin과정 진행
				if (PC != nullptr) 
				{
					PC->OnLogin(IDInput->GetText(), PWInput->GetText());
					//ID저장 체크박스 활성화의 유무에 따라 다음번 로그인때 ID를 텍스트박스에 호출시킬지 말지 결정. 추후에 게임인스턴스나 세이브데이터에 저장후 불러오는 기능 넣어야됨.
				}
			}
			//ID길이 5자 미만작성시 에러출력
			else 
			{
				GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Recommend to ID Length is over five"));
			}
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
		}
	}
	//PW 없으면 에러 출력
	else 
	{
		GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
	}
	*/
}
