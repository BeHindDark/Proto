// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Online.h"

DECLARE_LOG_CATEGORY_EXTERN(Proto,Log,All);

//로그가 출력되는 함수이름 및 위치를 알려줌
#define LINE_INFO (FString(__FUNCTION__) + TEXT("(") + FString::FromInt(__LINE__) + TEXT(")"))

#define CHECK_LOG(Expr) {UE_LOG(Proto, Error, TEXT("%s ASSERTION : %s"), *LINE_INFO, *FString::Printf(TEXT("'"#Expr"'")))}

//UE_LOG(Proto, Warning, TEXT("%s / %s : DetailErrorMessage"), *LINE_INFO, *GetNameSafe(this));