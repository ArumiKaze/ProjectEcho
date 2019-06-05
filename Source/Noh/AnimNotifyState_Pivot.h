#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NohCharacter.h"
#include "AnimNotifyState_Pivot.generated.h"

USTRUCT(BlueprintType)
struct FPivotParams
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pivotdirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_MOVEMENTDIRECTION completedmovementdirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float completedstarttime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_MOVEMENTDIRECTION interruptedmovementdirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float interruptedstarttime;
};

UCLASS()
class NOH_API UAnimNotifyState_Pivot : public UAnimNotifyState
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintCallable)
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;

public:

	UAnimNotifyState_Pivot();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPivotParams pivotparams;
};