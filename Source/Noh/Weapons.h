#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons.generated.h"

UCLASS()
class NOH_API AWeapons : public AActor
{
	GENERATED_BODY()

private:

protected:

	FName m_weaponname;

public:

	AWeapons();
	AWeapons(FName weaponname);

	virtual AWeapons* GetWeapon();

	virtual void Unsheath();

	virtual void Sheath();
};