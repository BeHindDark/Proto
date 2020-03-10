// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_Main.h"
#include "Pawn_Showcase.h"
#include "Pcon_Main.h"

AGM_Main::AGM_Main()
{
	DefaultPawnClass = APawn_Showcase::StaticClass();
	PlayerControllerClass = APcon_Main::StaticClass();
}