// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_AnimInstance.h"

UPC_AnimInstance::UPC_AnimInstance()
{
	Speed = 0.0f;
	Direction = 0.0f;
	DiagonalMode = false;
	HorizentalMode = false;
}
void UPC_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	//auto Pawn = Cast<Pawn클래스명>(TryGetPawnOwner())으로 바꿀것
	auto Pawn = TryGetPawnOwner();
	if (IsValid(Pawn))
	{
		Direction=(Pawn->GetActorForwardVector().ToOrientationRotator() - Pawn->GetVelocity().ToOrientationRotator()).Yaw;
		Speed = Pawn->GetVelocity().Size();
		//DiagonalMode=Pawn.DiagonalMode;
		//HorizentalMode=Pawn.HorizentalMode;
	}
}