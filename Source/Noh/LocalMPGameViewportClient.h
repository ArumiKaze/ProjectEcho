// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameViewportClient.h"
#include "LocalMPGameViewportClient.generated.h"

/**
 * 
 */
UCLASS()
class NOH_API ULocalMPGameViewportClient : public UGameViewportClient
{
	GENERATED_BODY()

	virtual bool InputKey(const FInputKeyEventArgs& EventArgs) override;
	virtual bool InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples = 1, bool bGamepad = false) override;
};
