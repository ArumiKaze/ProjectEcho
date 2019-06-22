#include "AnimNotify_Footstep.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "NohCharacter.h"

UAnimNotify_Footstep::UAnimNotify_Footstep()
{
	static ConstructorHelpers::FObjectFinder<USoundBase> soundbase(TEXT("/Game/MainCharacter/Audio/Footsteps/Footstep_Cue"));
	if (soundbase.Object) {
		sound = soundbase.Object;
	}
	footsteptype = E_FOOTSTEPTYPE::FST_STEP;
	volumemultiplier = 1.0f;
	pitchmultiplier = 1.0f;
	attachpointname = TEXT("");
}

void UAnimNotify_Footstep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL && MeshComp->IsVisible())
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			nohinstance->SetAnimNotify_Footstep(sound, MeshComp, footsteptype, volumemultiplier, pitchmultiplier, attachpointname);
		}
	}
}