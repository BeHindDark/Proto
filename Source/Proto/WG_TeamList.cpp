// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_TeamList.h"
#include "Components/VerticalBox.h"


void UWG_TeamList::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerListSpace->ClearChildren();
	
}