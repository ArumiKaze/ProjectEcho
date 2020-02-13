#include "AnimNotify_RaiseKatanaFinish.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UAnimNotify_RaiseKatanaFinish::UAnimNotify_RaiseKatanaFinish()
{
}

void UAnimNotify_RaiseKatanaFinish::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			nohinstance->AnimNotify_RaiseKatanaFinish();
		}
	}
}