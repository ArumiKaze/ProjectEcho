#include "Katana.h"

AKatana::AKatana()
	:AWeapons{ "katana", 1, 1.0f }
{
	if (GetWorld())
	{
		//Load Katana and Saya blueprint
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
	}
}

void AKatana::Debugprint() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Debug katana"));
}

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

void AKatana::Unsheath()
{
	ACharacter* CharacterReference{ UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) };
	ANohCharacter* NohReference{ Cast<ANohCharacter>(CharacterReference) };
	if (NohReference)
	{
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

void AKatana::weaponAction(int combophase)
{
}