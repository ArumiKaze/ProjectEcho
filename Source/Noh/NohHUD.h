#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NohHUD.generated.h"

UCLASS()
class NOH_API ANohHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	ANohHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> weaponswitchclass;

	class UUserWidget* widget_weaponswitch;

	void hud_weaponswitch(bool b_isswitching);
	
	virtual void BeginPlay() override;
};
