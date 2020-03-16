// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Login.h"
#include "PC_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


void UWG_Login::NativeConstruct() {
	Super::NativeConstruct();
	
	IDInput->OnTextCommitted.AddDynamic(this, &UWG_Login::IDCommitted);
	PWInput->OnTextCommitted.AddDynamic(this, &UWG_Login::PassWordCommitted);
	LoginButton->OnClicked.AddDynamic(this, &UWG_Login::OnLoginClicked);
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

void UWG_Login::OnLoginSession() {
	
	//ID길이 5자 이상 제한
	if (IDInput->GetText().ToString().Len() >= 5) {
		APC_Login* PC = Cast<APC_Login>(GetOwningPlayer());
		if (PC != nullptr) {

		}
	}
	//ID길이 5자 미만작성시
	else {

	}

}