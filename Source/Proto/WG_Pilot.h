// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WG_Pilot.generated.h"

/**
 * 
 */
UCLASS()
class PROTO_API UWG_Pilot : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UTextBlock* ClassNameText;

	UPROPERTY(BlueprintReadWrite, meta=(BindWidget))
	class UProgressBar* HPProgressBar;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Connection")
	TWeakObjectPtr<class UDamageControlSystem> DamageControlSystem_Ref;

public:
	UFUNCTION(BlueprintCallable, Category="Custom|Widget")
	void ConnectDamageControlSystem(class UDamageControlSystem* NewSystem);

	UFUNCTION(BlueprintCallable, Category="Custom|Widget")
	void SetClassName(const FName& ConnectedClassName);

protected:
	void OnHPIsChanged();
};
