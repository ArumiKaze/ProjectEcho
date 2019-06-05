#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"
#include "NohCharacter.h"
#include "AnimNotify_Footstep.generated.h"

UCLASS()
class NOH_API UAnimNotify_Footstep : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
public:
	UAnimNotify_Footstep();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USoundBase* sound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_FOOTSTEPTYPE footsteptype;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float volumemultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float pitchmultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName attachpointname;
};
