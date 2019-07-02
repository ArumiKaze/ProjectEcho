#include "Weapons.h"

AWeapons::AWeapons()
	: m_weaponname{ "" }
{
}
AWeapons::AWeapons(FName weaponname)
	: m_weaponname{ weaponname }
{
}

AWeapons* AWeapons::GetWeapon()
{
	return nullptr;
}

void AWeapons::Unsheath()
{
}

void AWeapons::Sheath()
{
}