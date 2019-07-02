#include "Katana.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"

AKatana::AKatana()
	:AWeapons{ "katana" }
{
	if (GetWorld())
	{
		//Current katana state, starts at noto
		katanastate = E_KATANASTATE::KS_NOTO;

		//Load Katana and Saya blueprint objects
		static ConstructorHelpers::FObjectFinder<UClass> KatanaBlueprint(TEXT("Class'/Game/Weapons/Katana/BP_Katana.BP_Katana_C'"));
		if (KatanaBlueprint.Object) {
			bp_katana = KatanaBlueprint.Object;
		}
		static ConstructorHelpers::FObjectFinder<UClass> SayaBlueprint(TEXT("Class'/Game/Weapons/Katana/BP_Saya.BP_Saya_C'"));
		if (SayaBlueprint.Object) {
			bp_saya = SayaBlueprint.Object;
		}

		//Load animation montages
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nbattouidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/N_Battou_Idle_Montage"));
		if (animmontage_nbattouidle.Object) {
			nbattou_idle = animmontage_nbattouidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nbattoumoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/N_Battou_Moving_Montage"));
		if (animmontage_nbattoumoving.Object) {
			nbattou_moving = animmontage_nbattoumoving.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nkamaenotoidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Idle_Kamae_Noto"));
		if (animmontage_nkamaenotoidle.Object) {
			nkamae_noto_idle = animmontage_nkamaenotoidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nkamaenotomoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Moving_Kamae_Noto"));
		if (animmontage_nkamaenotomoving.Object) {
			nkamae_noto_moving = animmontage_nkamaenotomoving.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nkamaechiburinotoidle(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Idle_Kamae_ChiburiNoto"));
		if (animmontage_nkamaechiburinotoidle.Object) {
			nkamae_chiburinoto_idle = animmontage_nkamaechiburinotoidle.Object;
		}
		static ConstructorHelpers::FObjectFinder<UAnimMontage> animmontage_nkamaechiburinotomoving(TEXT("/Game/MainCharacter/Animations/Combat/Katana/Noto/N_Moving_Kamae_ChiburiNoto"));
		if (animmontage_nkamaechiburinotomoving.Object) {
			nkamae_chiburinoto_moving = animmontage_nkamaechiburinotomoving.Object;
		}
	}
}

//---Get Katana---//
AWeapons* AKatana::GetWeapon()
{
	ACharacter* NohReference{ UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) };
	AKatana* katana{ nullptr };
	if (NohReference && bp_katana)
	{
		FActorSpawnParameters spawnparams;
		spawnparams.Owner = NohReference;
		katana = GetWorld()->SpawnActor<AKatana>(bp_katana, FVector{0.0f, 0.0f, 0.0f}, FRotator{ 0.0f, 0.0f, 0.0f }, spawnparams);
		AActor* saya{ GetWorld()->SpawnActor<AActor>(bp_saya, FVector{ 0.0f, 0.0f, 0.0f }, FRotator{ 0.0f, 0.0f, 0.0f }, spawnparams) };
		katana->AttachToComponent(NohReference->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_pelvis");
		saya->AttachToComponent(NohReference->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "socket_pelvis");
	}
	return katana;
}

//---Unsheath Katana---//
void AKatana::Unsheath()
{
	ANohCharacter* NohReference{ Cast<ANohCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) };
	if (NohReference)
	{
		katanastate = E_KATANASTATE::KS_KAMAE;
		NohReference->SetActiveWeapon(AWeapons::m_weaponname);
		if (!NohReference->GetIsMoving())
		{
			NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nbattou_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
		else
		{
			NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nbattou_moving, 1.5f, EMontagePlayReturnType::MontageLength, 0.0f, true);
		}
	}
}

//---Sheath Katana---//
void AKatana::Sheath()
{
	ANohCharacter* NohReference{ Cast<ANohCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)) };
	if (NohReference)
	{
		switch (katanastate)
		{
		case E_KATANASTATE::KS_KAMAE:
			if (NohReference->GetEnemyHit())
			{
				if (NohReference->GetIsMoving())
				{
					NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nkamae_chiburinoto_moving, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
				else
				{
					NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nkamae_chiburinoto_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
			else
			{
				if (NohReference->GetIsMoving())
				{
					NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nkamae_noto_moving, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
				else
				{
					NohReference->GetMesh()->GetAnimInstance()->Montage_Play(nkamae_noto_idle, 1.0f, EMontagePlayReturnType::MontageLength, 0.0f, true);
				}
			}
			break;
		case E_KATANASTATE::KS_HIDARIJOUHOU:
			break;
		case E_KATANASTATE::KS_MIGIJOUHOU:
			break;
		case E_KATANASTATE::KS_HIDARIKESA:
			break;
		case E_KATANASTATE::KS_MIGIKESA:
			break;
		case E_KATANASTATE::KS_SHOMEN:
			break;
		case E_KATANASTATE::KS_TSUKI:
			break;
		}

		katanastate = E_KATANASTATE::KS_NOTO;
	}
}