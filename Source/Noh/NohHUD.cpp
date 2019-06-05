// Fill out your copyright notice in the Description page of Project Settings.

#include "NohHUD.h"
#include "UI.h"
#include "UObject/ConstructorHelpers.h"

ANohHUD::ANohHUD()
{
	static ConstructorHelpers::FClassFinder<UUI> WidgetAsset(TEXT("/Game/UI/BP_WeaponSwitch"));
	if (WidgetAsset.Succeeded())
	{
		/** Assign the class of the loaded asset to the WigetClass variable, which is a "subclass" of UUserWidget : Which our asset class is */
		weaponswitchclass = WidgetAsset.Class;
	}
}

void ANohHUD::hud_weaponswitch(bool b_isswitching)
{
	if (b_isswitching)
	{
		if (weaponswitchclass != nullptr)
		{
			widget_weaponswitch = CreateWidget<UUserWidget>(GetWorld(), weaponswitchclass);
			if (widget_weaponswitch != nullptr)
			{
				widget_weaponswitch->AddToViewport();
				widget_weaponswitch->SetKeyboardFocus();
			}
		}
	}
	else
	{
		if (weaponswitchclass != nullptr)
		{
			if (widget_weaponswitch != nullptr)
			{
				widget_weaponswitch->RemoveFromViewport();
			}
		}
	}
}

void ANohHUD::BeginPlay()
{
}
