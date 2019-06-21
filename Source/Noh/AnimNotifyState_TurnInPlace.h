#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "AnimNotifyState_TurnInPlace.generated.h"

UCLASS()
class NOH_API UAnimNotifyState_TurnInPlace : public UAnimNotifyState
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration) override;

	UFUNCTION(BlueprintCallable)
	virtual void NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime) override;

	UFUNCTION(BlueprintCallable)
	virtual void NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation) override;
	
public:

	UAnimNotifyState_TurnInPlace();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCurveFloat* rotationcurveasset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isrightturn;
};
