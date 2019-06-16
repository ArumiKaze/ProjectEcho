#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "Katana.generated.h"

UCLASS()
class NOH_API AKatana : public AWeapons
{
	GENERATED_BODY()

private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKatana> bp_katana;

protected:

	UFUNCTION(BlueprintCallable)
	virtual void weaponAction(int combophase) override;

public:

	AKatana();

	UFUNCTION(BlueprintCallable)
	virtual void Debugprint() const override;

	UFUNCTION(BlueprintCallable)
	virtual AWeapons* GetWeapon() override;
};
