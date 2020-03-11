// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Main.h"
#include "Pn_Showcase.h"
#include "PC_Main.h"

AGM_Main::AGM_Main()
{
	DefaultPawnClass = APn_Showcase::StaticClass();
	PlayerControllerClass = APC_Main::StaticClass();
}