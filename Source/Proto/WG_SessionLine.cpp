// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_SessionLine.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"

void UWG_SessionLine::NativeConstruct()
{
	Super::NativeConstruct();
	
	if(bIsTitle)
	{
		//배경을 살짝 어둡게
		//BackgroundBorder->SetBrushColor(FLinearColor(0.0f, 0.0f, 0.0f, 0.5f));
		PlayerSeatText->SetText(FText::FromString(TEXT("Player Seat")));
	}
	else
	{
		PlayerSeatText->SetText(FText::FromString(TEXT("/")));
	}
}

void UWG_SessionLine::NativeOnMouseEnter(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);
	
	if(!bIsTitle)
	{
		//배경을 밝게 하이라이트
		BackgroundBorder->SetBrushColor(FLinearColor(0.8f,1.0f,1.0f,1.0f));
	}
	
}

void UWG_SessionLine::NativeOnMouseLeave(const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	if(!bIsTitle)
	{
		
		//배경을 투명하게
		BackgroundBorder->SetBrushColor(FLinearColor(0.0f,0.0f,0.0f,0.0f));
		
	}
	
}


FReply UWG_SessionLine::NativeOnMouseButtonDown(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	
	return FReply::Unhandled();
}

FReply UWG_SessionLine::NativeOnMouseButtonDoubleClick(const FGeometry & InGeometry,const FPointerEvent & InMouseEvent)
{
	Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
	
	return FReply::Unhandled();
}
