#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "Enemy_Dummy.generated.h"

UCLASS()
class NOH_API AEnemy_Dummy : public AEnemy
{
	GENERATED_BODY()

protected:

	UFUNCTION(BlueprintCallable)
		virtual void onWeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
public:

	AEnemy_Dummy();
};
