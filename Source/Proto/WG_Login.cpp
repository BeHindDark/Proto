// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Login.h"
#include "PC_Login.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/EditableTextBox.h"


void UWG_Login::NativeConstruct() {
	Super::NativeConstruct();

	IDInput->OnTextCommitted.AddDynamic(this, &UWG_Login::IDCommitted);
	PWInput->OnTextCommitted.AddDynamic(this, &UWG_Login::PassWordCommitted);
	LoginButton->OnClicked.AddDynamic(this, &UWG_Login::OnLoginClicked);
	CreateAccountButton->OnClicked.AddDynamic(this, &UWG_Login::OnCreateAccountClicked);
	UseServerDataCheckBox->OnCheckStateChanged.AddDynamic(this, &UWG_Login::OnServerDataCheckBoxClicked);
	UseLocalDataCheckBox->OnCheckStateChanged.AddDynamic(this, &UWG_Login::OnLocalDataCheckBoxClicked);
}

void UWG_Login::IDCommitted(const FText& InText, ETextCommit::Type InCommitType) {
	if (InCommitType == ETextCommit::OnEnter) {
		PWInput->SetKeyboardFocus();
	}
}

void UWG_Login::PassWordCommitted(const FText& InText, ETextCommit::Type InCommitType) {
	if (InCommitType == ETextCommit::OnEnter) {
		UWG_Login::OnLoginSession();
	}
}

void UWG_Login::OnLoginClicked() {
	UWG_Login::OnLoginSession();
}

void UWG_Login::OnCreateAccountClicked()
{
	APC_Login* PC = Cast<APC_Login>(GetOwningPlayer());

	if (!IsValid(PC))
	{
		CHECK_LOG(!IsValid(PC));
	}
	else
	{
		SetVisibility(ESlateVisibility::Hidden);
		PC->ShowJoinWG(1);
	}
	
}



void UWG_Login::OnLoginSession() {
	//PW있어야됨
	if (PWInput->GetText().ToString().Len() != 0) {
		//ID길이 5자 이상 제한
		if (IDInput->GetText().ToString().Len() >= 5) {
			APC_Login* PC = Cast<APC_Login>(GetOwningPlayer());
			//ID 5자 이상, PW 존재라는 조건을 통과해야 OnLogin과정 진행
			if (PC != nullptr) {
				PC->OnLogin(IDInput->GetText(), PWInput->GetText());
				//ID저장 체크박스 활성화의 유무에 따라 다음번 로그인때 ID를 텍스트박스에 호출시킬지 말지 결정. 추후에 게임인스턴스나 세이브데이터에 저장후 불러오는 기능 넣어야됨.
				if (SaveIDCheckBox->GetCheckedState() == ECheckBoxState::Checked) {

				}
			}
		}
		//ID길이 5자 미만작성시 에러출력
		else {
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Recommend to ID Length is over five"));
		}
	}
	//PW 없으면 에러 출력
	else {
		GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
	}
}

void UWG_Login::OnServerDataCheckBoxClicked(bool Checked) {
	//체크박스가 체크상태인지 아닌지에 따라 나머지 체크박스가 활성, 비활성화되는 코드
	if (Checked == true) {
		UseLocalDataCheckBox->bIsEnabled = false;
	}
	else {
		UseLocalDataCheckBox->bIsEnabled = true;
	}

	//서버체크박스가 체크된 상태에서 로컬체크박스를 체크할시 디버그출력후 로컬체크박스 체크해제
	if (UseLocalDataCheckBox->GetCheckedState() == ECheckBoxState::Checked) {
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Purple, TEXT("If you chechked 'Load User Data from Local ', you don't check to 'Load User Data from Server'."));
		UseServerDataCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UWG_Login::OnLocalDataCheckBoxClicked(bool Checked) {
	//체크박스가 체크상태인지 아닌지에 따라 나머지 체크박스가 활성, 비활성화되는 코드
	if (Checked == true) {
		UseServerDataCheckBox->bIsEnabled = false;
	}
	else {
		UseServerDataCheckBox->bIsEnabled = true;
	}

	//로컬체크박스가 체크된 상태에서 서버체크박스를 체크할시 디버그출력후 서버체크박스 체크해제
	if (UseServerDataCheckBox->GetCheckedState() == ECheckBoxState::Checked) {
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Purple, TEXT("If you chechked 'Load User Data from Server ', you don't check to 'Load User Data from Local'."));
		UseLocalDataCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}