#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_Sheathing.generated.h"

UCLASS()
class NOH_API UAnimNotifyState_Sheathing : public UAnimNotifyState
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;

	UFUNCTION(BlueprintCallable)
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;

public:

	UAnimNotifyState_Sheathing();
};
