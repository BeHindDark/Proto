// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Login.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"


void UWG_Login::NativeConstruct() {
	Super::NativeConstruct();

	LoginButton->OnClicked.AddDynamic(this, &UWG_Login::OnLoginClicked);
	
	IDInput = Cast<UEditableTextBox>(GetWidgetFromName(TEXT("IDInput")));
	if (IDInput != nullptr) {
		IDInput->OnTextCommitted.RemoveDynamic(this, UWG_Login::IDCommitted);
		IDInput->OnTextCommitted.AddDynamic(this, &UWG_Login::IDCommitted);
	}

}

void UWG_Login::IDCommitted() {

}

void UWG_Login::PassWordCommitted() {

}

void UWG_Login::OnLoginClicked() {

}