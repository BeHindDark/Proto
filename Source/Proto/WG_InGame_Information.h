// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Proto.h"
#include "Blueprint/UserWidget.h"
#include "WG_InGame_Information.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_InGame_Information : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void SetHP(UTextBlock* txtTMP, int hp);

	UFUNCTION()
	void SetUseWeapon(bool used1, bool used2, bool used3);

protected:
	//Weapone Box
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* boxWeaponHP1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* boxWeaponHP2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class USizeBox* boxWeaponHP3;

	//HP Text
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtBodyHP;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponHP1;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponHP2;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
		class UTextBlock* txtWeaponHP3;

protected:
	bool UseWeapon1;
	bool UseWeapon2;
	bool UseWeapon3;
};
