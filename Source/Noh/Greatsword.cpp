#include "Greatsword.h"



AGreatsword::AGreatsword()
	:AWeapons{ "greatsword", 200, 0.5f }
{
	//OnActorBeginOverlap.AddDynamic(this, &AGreatsword::onWeaponPickup);
}

/*
void AGreatsword::onWeaponPickup(AActor * MyOverlappedActor, AActor * OtherActor)
{
	if (spawngreatsword)
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
				m_nohcharacter->addWeaponInventory(GetWorld()->SpawnActor<AGreatsword>(spawngreatsword, spawnlocation, rotator, spawnparams));
				Destroy();
			}
		}
	}
}
*/

void AGreatsword::weaponAction(int combophase)
{
	/*
	m_nohcharacter = Cast<ANohCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (m_nohcharacter)
	{
		FVector Force;

		switch (combophase)
		{
		case 1:
			Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 200.0f, m_nohcharacter->GetActorForwardVector().Y * 200.0f, 100.0f };
			break;
		case 2:
			Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 400.0f, m_nohcharacter->GetActorForwardVector().Y * 400.0f, 100.0f };
			break;
		case 3:
			Force = FVector{ m_nohcharacter->GetActorForwardVector().X * 200.0f, m_nohcharacter->GetActorForwardVector().Y * 200.0f, 100.0f };
			break;
		default:
			break;
		}
		m_nohcharacter->LaunchCharacter(Force, true, true);
	}
	*/
}

FName AGreatsword::getweaponsheathSocket(bool sheathed) const
{
	if (sheathed)
	{
		return TEXT("weaponholster_back");
	}
	else
	{
		return TEXT("socket_greatsword");
	}
}

void AGreatsword::Debugprint() const
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Debug greatsword"));
}