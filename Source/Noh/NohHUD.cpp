#include "NohHUD.h"
#include "UI.h"
#include "UObject/ConstructorHelpers.h"
#include "NohCharacter.h"

ANohHUD::ANohHUD()
{
	/*
	static ConstructorHelpers::FClassFinder<UUI> WidgetAsset(TEXT("/Game/UI/BP_WeaponSwitch"));
	if (WidgetAsset.Succeeded())
	{
		//Assign the class of the loaded asset to the WigetClass variable, which is a "subclass" of UUserWidget : Which our asset class is
		weaponswitchclass = WidgetAsset.Class;
	}
	*/
}

void ANohHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ANohHUD::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ANohHUD::DrawHUD()
{
	Super::DrawHUD();
}

void ANohHUD::CreateWheel()
{
	if (classwidget_wheel)
	{
		widget_wheel = CreateWidget<UUI>(GetOwningPlayerController(), classwidget_wheel);
		if (widget_wheel)
		{
			widget_wheel->AddToPlayerScreen();
			//widget_wheel->SetKeyboardFocus();
		}
	}
}

void ANohHUD::UpdateWheel(int index)
{
	if (widget_wheel)
	{
		widget_wheel->UpdateWheel(index);
	}
}

void ANohHUD::HideWheel()
{
	if (widget_wheel)
	{
		widget_wheel->HideWheel();
	}
}

/*
void ANohHUD::hud_weaponwheel(bool b_isswitching)
{
	if (b_isswitching)
	{
		if (hud_weaponwheelclass != nullptr)
		{
			widget_weaponwheel = CreateWidget<UUserWidget>(GetWorld(), hud_weaponwheelclass);
			if (widget_weaponwheel != nullptr)
			{
				widget_weaponwheel->AddToViewport();
				widget_weaponwheel->SetKeyboardFocus();
			}
		}
	}
	else
	{
		if (hud_weaponwheelclass != nullptr)
		{
			if (widget_weaponwheel != nullptr)
			{
				widget_weaponwheel->RemoveFromViewport();
			}
		}
	}
}
*/