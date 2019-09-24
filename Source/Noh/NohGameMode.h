// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "NohGameMode.generated.h"

UCLASS(minimalapi)
class ANohGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ANohGameMode();

	virtual void BeginPlay() override;
};



