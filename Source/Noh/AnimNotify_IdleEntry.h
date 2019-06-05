#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "NohCharacter.h"
#include "AnimNotify_IdleEntry.generated.h"

UCLASS()
class NOH_API UAnimNotify_IdleEntry : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

public:
	UAnimNotify_IdleEntry();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	E_IDLEENTRYSTATE an_idleentrystate;
};
