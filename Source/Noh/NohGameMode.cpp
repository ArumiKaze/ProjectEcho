// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#include "NohGameMode.h"
#include "NohCharacter.h"
#include "NohHUD.h"
#include "UObject/ConstructorHelpers.h"

ANohGameMode::ANohGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/MainCharacter/BP_NohMann"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	HUDClass = ANohHUD::StaticClass();
}
