#include "AnimNotifyState_Pivot.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"

UAnimNotifyState_Pivot::UAnimNotifyState_Pivot()
{
	pivotparams.pivotdirection = 0.0f;
	pivotparams.completedmovementdirection = E_MOVEMENTDIRECTION::MD_FORWARDS;
	pivotparams.completedstarttime = 0.0f;
	pivotparams.interruptedmovementdirection = E_MOVEMENTDIRECTION::MD_FORWARDS;
	pivotparams.interruptedstarttime = 0.0f;
}

void UAnimNotifyState_Pivot::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			nohinstance->SetPivotParams(pivotparams.pivotdirection, pivotparams.completedmovementdirection, pivotparams.completedstarttime, pivotparams.interruptedmovementdirection, pivotparams.interruptedstarttime);
		}
	}
}