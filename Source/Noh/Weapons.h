//Abstract class for Weapons//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "INTERFACE_Weapons.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Weapons.generated.h"

UCLASS()
class NOH_API AWeapons : public AActor, public IINTERFACE_Weapons
{
	GENERATED_BODY()

private:

	FName m_weapontype;
	int m_damage;
	float m_weaponattackspeed;

protected:

	UPROPERTY()
		class ANohCharacter* m_nohcharacter;



	virtual void onWeaponPickup(AActor* MyOverlappedActor, AActor* OtherActor);

	virtual void weaponAction(int combophase);

public:

	AWeapons();
	AWeapons(FName weapontype, int damage, float weaponattackspeed);



	virtual void debugprint() const;

	virtual FName getweaponsheathSocket(bool sheathed) const;

	UFUNCTION(BlueprintCallable)
		int getDamage() const;

	UFUNCTION(BlueprintCallable)
		FName getweaponType() const;

	UFUNCTION(BlueprintCallable)
		float getAttackspeed() const;
};