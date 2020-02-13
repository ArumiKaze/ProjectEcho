#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "NohHUD.generated.h"

UCLASS()
class NOH_API ANohHUD : public AHUD
{
	GENERATED_BODY()

private:

	UPROPERTY()
	class UUI* widget_wheel;
	
public:

	ANohHUD();

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
	TSubclassOf<class UUserWidget> classwidget_wheel;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	virtual void DrawHUD() override;

	void CreateWheel();
	
	void UpdateWheel(int index);

	void HideWheel();
};
