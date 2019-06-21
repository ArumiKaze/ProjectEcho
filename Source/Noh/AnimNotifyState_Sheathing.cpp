#include "AnimNotifyState_Sheathing.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UAnimNotifyState_Sheathing::UAnimNotifyState_Sheathing()
{
}

void UAnimNotifyState_Sheathing::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			//Attach or detach weapon to respective socket
			nohinstance->AnimNotifyState_Sheathing_WeaponAttachDettach();
		}
	}
}

void UAnimNotifyState_Sheathing::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			//End sheathing
			nohinstance->AnimNotifyState_Sheathing_End();
		}
	}
}