#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "Sword.generated.h"

UCLASS()
class NOH_API ASword : public AWeapons
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class ASword> spawnsword;

protected:

	UFUNCTION(BlueprintCallable)
		virtual void onWeaponPickup(AActor* MyOverlappedActor, AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
		virtual void weaponAction(int combophase) override;

public:

	ASword();

	UFUNCTION(BlueprintCallable)
		virtual void debugprint() const override;

	UFUNCTION(BlueprintCallable)
		virtual FName getweaponsheathSocket(bool sheathed) const override;
};
