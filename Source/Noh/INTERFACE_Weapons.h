//Interface class for Weapons//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INTERFACE_Weapons.generated.h"

UINTERFACE(BlueprintType)
class UINTERFACE_Weapons : public UInterface
{
	GENERATED_BODY()
};

class NOH_API IINTERFACE_Weapons
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void onWeaponPickup(AActor* MyOverlappedActor, AActor* OtherActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void weaponAction(int combophase);

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void debugprint() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		FName getweaponsheathSocket(bool sheathed) const;
};