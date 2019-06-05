#include "AnimNotifyState_TurnInPlace.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "NohCharacter.h"

UAnimNotifyState_TurnInPlace::UAnimNotifyState_TurnInPlace()
{
	rotationcurveasset = nullptr;
	montage = nullptr;
	isrightturn = false;
}

void UAnimNotifyState_TurnInPlace::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			//Set values in character for turn in place
			nohinstance->SetTurnInPlace(montage, true, true, isrightturn);
		}
	}
}

void UAnimNotifyState_TurnInPlace::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		if (nohinstance != NULL)
		{
			//Get difference between the curve float value at the current frame and previous frame, then add that to character rotation
			float previousvalue{ rotationcurveasset->GetFloatValue(MeshComp->GetAnimInstance()->Montage_GetPosition(montage)) };
			float currentvalue{ rotationcurveasset->GetFloatValue(MeshComp->GetAnimInstance()->Montage_GetPosition(montage) + (MeshComp->GetAnimInstance()->Montage_GetPlayRate(montage) * FrameDeltaTime)) };
			nohinstance->AddCharacterRotation(FRotator{ 0.0f, currentvalue - previousvalue, 0.0f });

			//check if actor has any velocity, if it does, stop current montage
			if (UKismetMathLibrary::NotEqual_VectorVector(nohinstance->GetVelocity(), FVector{ 0.0f, 0.0f, 0.0f }, 0.0001f))
			{
				MeshComp->GetAnimInstance()->Montage_Stop(0.2f, montage);
			}
		}
	}
}

void UAnimNotifyState_TurnInPlace::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
	if (MeshComp != NULL && MeshComp->GetOwner() != NULL)
	{
		ANohCharacter* nohinstance{ Cast<ANohCharacter>(MeshComp->GetOwner()) };
		//Reset values
		if (nohinstance != NULL)
		{
			nohinstance->SetTurnInPlace(montage, false, false, false);
		}
	}
}