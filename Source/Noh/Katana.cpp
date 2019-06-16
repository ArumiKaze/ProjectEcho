#include "Katana.h"

AKatana::AKatana()
	:AWeapons{ "katana", 1, 1.0f }
{
	if (GetWorld())
	{
		static ConstructorHelpers::FObjectFinder<UClass> KatanaBlueprint(TEXT("Class'/Game/Weapons/Katana/BP_Katana.BP_Katana_C'"));
		if (KatanaBlueprint.Object) {
			bp_katana = KatanaBlueprint.Object;
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
		//katana->AttachToComponent(NohReference->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "weaponholster");
		Debugprint();
	}
	return katana;
}

void AKatana::weaponAction(int combophase)
{
}