// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PC_AnimInstance.generated.h"

/**
 *
 */
UCLASS()
class PROTO_API UPC_AnimInstance : public UAnimInstance
{
    GENERATED_BODY()


public:
    UPC_AnimInstance();
    virtual void NativeUpdateAnimation(float DeltaSeconds) override;

private:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
        float Speed;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
        float Direction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
        bool DiagonalMode;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Pawn, Meta = (AllowPrivateAccess = true))
        bool HorizentalMode;
};


