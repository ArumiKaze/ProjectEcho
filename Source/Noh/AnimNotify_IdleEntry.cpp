#include "AnimNotify_IdleEntry.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UAnimNotify_IdleEntry::UAnimNotify_IdleEntry()
{
	an_idleentrystate = E_IDLEENTRYSTATE::N_IDLE;
}


void UAnimNotify_IdleEntry::Notify(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			nohinstance->SetIdleEntryState(an_idleentrystate);
		}
	}
}