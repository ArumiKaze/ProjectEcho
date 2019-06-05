//Interface class for Enemy//

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "INTERFACE_Enemy.generated.h"

UINTERFACE(BlueprintType)
class UINTERFACE_Enemy : public UInterface
{
	GENERATED_BODY()
};

class NOH_API IINTERFACE_Enemy
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void onWeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
