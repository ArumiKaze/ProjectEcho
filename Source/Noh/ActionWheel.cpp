#include "ActionWheel.h"
#include "Components/Overlay.h"
#include "Components/Image.h"

UActionWheel::UActionWheel(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UActionWheel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UActionWheel::UpdateWheel(int index)
{
	if (overlay_wheel && index > 0)
	{
		if (overlay_wheel->Visibility == ESlateVisibility::Hidden)
		{
			overlay_wheel->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UActionWheel::HideWheel()
{
	if (overlay_wheel)
	{
		overlay_wheel->SetVisibility(ESlateVisibility::Hidden);
	}
}
