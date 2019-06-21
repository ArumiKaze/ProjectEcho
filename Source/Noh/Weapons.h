//Abstract class for Weapons//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Animation/AnimInstance.h"
#include "Weapons.generated.h"

UCLASS()
class NOH_API AWeapons : public AActor
{
	GENERATED_BODY()

private:

	FName m_weapontype;
	int m_damage;
	float m_weaponattackspeed;

protected:

	virtual void weaponAction(int combophase);

public:

	AWeapons();
	AWeapons(FName weapontype, int damage, float weaponattackspeed);

	virtual void Debugprint() const;

	virtual AWeapons* GetWeapon();

	virtual void Unsheath();

	virtual FName getweaponsheathSocket(bool sheathed) const;

	UFUNCTION(BlueprintCallable)
		int getDamage() const;

	UFUNCTION(BlueprintCallable)
		FName getweaponType() const;

	UFUNCTION(BlueprintCallable)
		float getAttackspeed() const;
};