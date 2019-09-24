#include "LocalMPGameViewportClient.h"

bool ULocalMPGameViewportClient::InputKey(const FInputKeyEventArgs& EventArgs)
{
	if (EventArgs.IsGamepad())
	{
		// Map the gamepad to the next player index (so 1st controller is player index 1, etc.)
		return Super::InputKey(FInputKeyEventArgs(EventArgs.Viewport, EventArgs.ControllerId + 1, EventArgs.Key, EventArgs.Event, EventArgs.AmountDepressed, EventArgs.IsGamepad()));
	}
	else
	{
		return Super::InputKey(EventArgs);
	}
}
bool ULocalMPGameViewportClient::InputAxis(FViewport* Viewport, int32 ControllerId, FKey Key, float Delta, float DeltaTime, int32 NumSamples, bool bGamepad)
{
	if (bGamepad)
	{
		// Map the gamepad to the next player index (so 1st controller is player index 1, etc.)
		return Super::InputAxis(Viewport, ControllerId + 1, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
	else
	{
		return Super::InputAxis(Viewport, ControllerId, Key, Delta, DeltaTime, NumSamples, bGamepad);
	}
}