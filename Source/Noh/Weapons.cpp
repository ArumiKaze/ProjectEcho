#include "Weapons.h"



AWeapons::AWeapons()
	: m_weapontype{ "" }, m_damage{ 0 }, m_weaponattackspeed{ 0.0f }
{
}
AWeapons::AWeapons(FName weapontype, int damage, float weaponattackspeed)
	: m_weapontype{ weapontype }, m_damage{ damage }, m_weaponattackspeed{ weaponattackspeed }
{
}



void AWeapons::onWeaponPickup(AActor* MyOverlappedActor, AActor* OtherActor)
{
}



void AWeapons::weaponAction(int combophase)
{
}



void AWeapons::debugprint() const
{
}



FName AWeapons::getweaponsheathSocket(bool sheathed) const
{
	return TEXT("");
}



//Getters & Setters//
int AWeapons::getDamage() const
{
	return m_damage;
}
FName AWeapons::getweaponType() const
{
	return m_weapontype;
}
float AWeapons::getAttackspeed() const
{
	return m_weaponattackspeed;
}