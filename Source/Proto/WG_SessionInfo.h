// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_SessionInfo.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_SessionInfo : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
public:
	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UEditableTextBox* SessionNameTextBox;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UEditableTextBox* HostNameTextBox;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class UMultiLineEditableText* DescriptionMLEditableText;

	UPROPERTY(BlueprintReadWrite,meta=(BindWidget))
	class USizeBox* ScrollSize;

protected:
	UPROPERTY(EditInstanceOnly,Category = UI,meta=(AllowPrivateAccess = true))
	/**	�� SessionInfo ������ ������������ ����
	*	@default value	false
	*	������ ����ų� �����ϴ� �뵵�� ���� true
	*	false�� ���������� ���̴� �뵵�̸� ��� TextBox���� �����Ұ���������.
	*/
	bool bIsEditable = true;

public:
	/**	bIsEditable�� ���� �����ϰ�, �� ���ÿ� �°� textbox���� �����Ѵ�.
	*	@param	bool Editable	���� ������ bIsEditable�� ��
	*	@detail	false�� �� �ؽ�Ʈ�� �����Ұ���. true�� �Ϻ� �ؽ�Ʈ���� ��������
	*/
	void SetIsEditable(bool Editable);

	/**	��� EditableTextBox �� MultiLineEditableText�� ����.
	*/
	void ClearText();
};
