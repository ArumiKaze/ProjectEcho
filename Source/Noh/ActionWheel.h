#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ActionWheel.generated.h"

UCLASS()
class NOH_API UActionWheel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UOverlay* overlay_wheel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
		class UImage* image_cursor;

public:
	UActionWheel(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	void UpdateWheel(int index);

	void HideWheel();
};
