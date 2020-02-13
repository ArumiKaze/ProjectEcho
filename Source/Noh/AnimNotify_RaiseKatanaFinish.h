#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_RaiseKatanaFinish.generated.h"

UCLASS()
class NOH_API UAnimNotify_RaiseKatanaFinish : public UAnimNotify
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:

	UAnimNotify_RaiseKatanaFinish();

};
