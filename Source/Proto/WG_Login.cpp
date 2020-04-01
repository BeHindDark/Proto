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
	//PW�־�ߵ�
	if (PWInput->GetText().ToString().Len() != 0) {
		//ID���� 5�� �̻� ����
		if (IDInput->GetText().ToString().Len() >= 5) {
			APC_Login* PC = Cast<APC_Login>(GetOwningPlayer());
			//ID 5�� �̻�, PW ������ ������ ����ؾ� OnLogin���� ����
			if (PC != nullptr) {
				PC->OnLogin(IDInput->GetText(), PWInput->GetText());
				//ID���� üũ�ڽ� Ȱ��ȭ�� ������ ���� ������ �α��ζ� ID�� �ؽ�Ʈ�ڽ��� ȣ���ų�� ���� ����. ���Ŀ� �����ν��Ͻ��� ���̺굥���Ϳ� ������ �ҷ����� ��� �־�ߵ�.
				if (SaveIDCheckBox->GetCheckedState() == ECheckBoxState::Checked) {

				}
			}
		}
		//ID���� 5�� �̸��ۼ��� �������
		else {
			GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Recommend to ID Length is over five"));
		}
	}
	//PW ������ ���� ���
	else {
		GEngine->AddOnScreenDebugMessage(10, 10, FColor::Blue, TEXT("Input Password"));
	}
}

void UWG_Login::OnServerDataCheckBoxClicked(bool Checked) {
	//üũ�ڽ��� üũ�������� �ƴ����� ���� ������ üũ�ڽ��� Ȱ��, ��Ȱ��ȭ�Ǵ� �ڵ�
	if (Checked == true) {
		UseLocalDataCheckBox->bIsEnabled = false;
	}
	else {
		UseLocalDataCheckBox->bIsEnabled = true;
	}

	//����üũ�ڽ��� üũ�� ���¿��� ����üũ�ڽ��� üũ�ҽ� ���������� ����üũ�ڽ� üũ����
	if (UseLocalDataCheckBox->GetCheckedState() == ECheckBoxState::Checked) {
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Purple, TEXT("If you chechked 'Load User Data from Local ', you don't check to 'Load User Data from Server'."));
		UseServerDataCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}

void UWG_Login::OnLocalDataCheckBoxClicked(bool Checked) {
	//üũ�ڽ��� üũ�������� �ƴ����� ���� ������ üũ�ڽ��� Ȱ��, ��Ȱ��ȭ�Ǵ� �ڵ�
	if (Checked == true) {
		UseServerDataCheckBox->bIsEnabled = false;
	}
	else {
		UseServerDataCheckBox->bIsEnabled = true;
	}

	//����üũ�ڽ��� üũ�� ���¿��� ����üũ�ڽ��� üũ�ҽ� ���������� ����üũ�ڽ� üũ����
	if (UseServerDataCheckBox->GetCheckedState() == ECheckBoxState::Checked) {
		GEngine->AddOnScreenDebugMessage(10, 5, FColor::Purple, TEXT("If you chechked 'Load User Data from Server ', you don't check to 'Load User Data from Local'."));
		UseLocalDataCheckBox->SetCheckedState(ECheckBoxState::Unchecked);
	}
}