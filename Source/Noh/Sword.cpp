#include "Sword.h"


ASword::ASword()
	:AWeapons{ "sword", 100, 1.0f }
{
	//OnActorBeginOverlap.AddDynamic(this, &ASword::onWeaponPickup);
}

/*
void ASword::onWeaponPickup(AActor * MyOverlappedActor, AActor * OtherActor)
{
	if (spawnsword)
	{
		m_nohcharacter = Cast<ANohCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
		if (m_nohcharacter)
		{
			if (OtherActor == m_nohcharacter && MyOverlappedActor != m_nohcharacter->getWeapon())
			{
				FActorSpawnParameters spawnparams;
				spawnparams.Owner = m_nohcharacter;
				FRotator rotator{ 0.0f, 0.0f, 0.0f };
				FVector spawnlocation{ 0.0f, 0.0f, 0.0f };
				m_nohcharacter->addWeaponInventory(GetWorld()->SpawnActor<ASword>(spawnsword, spawnlocation, rotator, spawnparams));
				Destroy();
			}
		}
	}
}
*/

AWeapons* ASword::GetWeapon()
{
	ACharacter* NohReference{ UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) };
	ASword* katana{ nullptr };
	if (NohReference)
	{
		katana = GetWorld()->SpawnActor<ASword>(GetClass(), FVector{0.0f, 0.0f, 0.0f}, FRotator{0.0f, 0.0f, 0.0f}, FActorSpawnParameters());
		katana->AttachToComponent(NohReference->GetMesh(), FAttachmentTransformRules{ EAttachmentRule::SnapToTarget, true }, "weaponholster");
	}
	return katana;
}

void ASword::weaponAction(int combophase)
{
	/*
	m_nohcharacter = Cast<ANohCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (m_nohcharacter)
	{
		FVector Force;

		switch (combophase)
		{
		case 1:
			//Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 400.0f, m_nohcharacter->GetActorForwardVector().Y * 400.0f, 100.0f };
			break;
		case 2:
			//Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 800.0f, m_nohcharacter->GetActorForwardVector().Y * 800.0f, 100.0f };
			break;
		case 3:
			//Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 800.0f, m_nohcharacter->GetActorForwardVector().Y * 800.0f, 1000.0f };
			break;
		default:
			break;
		}
		m_nohcharacter->LaunchCharacter(Force, false, false);
	}
	*/
}

FName ASword::getweaponsheathSocket(bool sheathed) const
{
	if (sheathed)
	{
		return TEXT("weaponholster");
	}
	else
	{
		return TEXT("weaponarmed");
	}
}

void ASword::Debugprint() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Debug sword"));
}