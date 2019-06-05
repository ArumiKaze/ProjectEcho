#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "Greatsword.generated.h"

UCLASS()
class NOH_API AGreatsword : public AWeapons
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
		TSubclassOf<class AGreatsword> spawngreatsword;
	
protected:

	UFUNCTION(BlueprintCallable)
		virtual void onWeaponPickup(AActor* MyOverlappedActor, AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable)
		virtual void weaponAction(int combophase) override;

public:

	AGreatsword();

	UFUNCTION(BlueprintCallable)
		virtual void debugprint() const override;

	UFUNCTION(BlueprintCallable)
		virtual FName getweaponsheathSocket(bool sheathed) const override;
};
