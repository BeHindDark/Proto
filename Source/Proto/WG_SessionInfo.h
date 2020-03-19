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
	/**	이 SessionInfo 위젯이 편집용인지의 여부
	*	@default value	false
	*	세션을 만들거나 편집하는 용도일 때는 true
	*	false면 세션정보를 보이는 용도이며 모든 TextBox들이 편집불가능해진다.
	*/
	bool bIsEditable = true;

public:
	/**	bIsEditable의 값을 변경하고, 새 세팅에 맞게 textbox등을 설정한다.
	*	@param	bool Editable	새로 설정할 bIsEditable의 값
	*	@detail	false면 각 텍스트는 수정불가능. true면 일부 텍스트빼고 수정가능
	*/
	void SetIsEditable(bool Editable);

	/**	모든 EditableTextBox 및 MultiLineEditableText를 비운다.
	*/
	void ClearText();
};
