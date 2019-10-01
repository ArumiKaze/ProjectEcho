#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI.generated.h"

UCLASS()
class NOH_API UUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UOverlay* overlay_wheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	class UImage* image_cursor;

public:
	UUI(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateWheel(int index);

	void HideWheel();
};
