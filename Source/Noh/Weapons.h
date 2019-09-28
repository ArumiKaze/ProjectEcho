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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* mesh_weapon;

public:

	AWeapons();
	AWeapons(FName weaponname);

	virtual AWeapons* GetWeapon(class ACharacter*& nohref);

	virtual void Unsheath(class ACharacter*& nohref);

	virtual void Sheath(class ACharacter*& nohref);
};