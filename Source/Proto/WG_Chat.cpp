// Fill out your copyright notice in the Description page of Project Settings.


#include "WG_Chat.h"

void UWG_Chat::NativeConstruct()
{

}

void UWG_Chat::ChattingCommitted(const FText& Input, ETextCommit::Type InCommitType)
{
	if (InCommitType == ETextCommit::OnEnter)
	{
		if (Input.IsEmpty())
		{

		}
		else
		{

		}
	}
}
