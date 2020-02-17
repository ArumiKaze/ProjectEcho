#include "AnimNotify_LowerKatanaFinish.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UAnimNotify_LowerKatanaFinish::UAnimNotify_LowerKatanaFinish()
{
}

void UAnimNotify_LowerKatanaFinish::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			nohinstance->AnimNotify_LowerKatanaFinish();
		}
	}
}