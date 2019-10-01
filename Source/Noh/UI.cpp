#include "UI.h"
#include "Components/Overlay.h"
#include "Components/Image.h"

UUI::UUI(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
}

void UUI::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUI::UpdateWheel(int index)
{
	if (overlay_wheel && index > 0)
	{
		if (overlay_wheel->Visibility == ESlateVisibility::Hidden)
		{
			overlay_wheel->SetVisibility(ESlateVisibility::Visible);
		}
	}
}

void UUI::HideWheel()
{
	if (overlay_wheel)
	{
		overlay_wheel->SetVisibility(ESlateVisibility::Hidden);
	}
}
