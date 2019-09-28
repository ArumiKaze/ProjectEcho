#include "Weapons.h"

AWeapons::AWeapons()
	: m_weaponname{ "" }
{
}
AWeapons::AWeapons(FName weaponname)
	: m_weaponname{ weaponname }
{
	mesh_weapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));
}

AWeapons* AWeapons::GetWeapon(ACharacter*& nohref)
{
	return nullptr;
}

void AWeapons::Unsheath(ACharacter*& nohref)
{
}

void AWeapons::Sheath(ACharacter*& nohref)
{
}