#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "Katana.generated.h"

UCLASS()
class NOH_API AKatana : public AWeapons
{
	GENERATED_BODY()

private:

	//Katana and Saya blueprints
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKatana> bp_katana;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> bp_saya;

	//Animation montages
	UPROPERTY()
	UAnimMontage* nbattou_idle;
	UPROPERTY()
	UAnimMontage* nbattou_moving;

protected:

	UFUNCTION(BlueprintCallable)
	virtual void weaponAction(int combophase) override;

public:

	AKatana();

	UFUNCTION(BlueprintCallable)
	virtual void Debugprint() const override;

	UFUNCTION(BlueprintCallable)
	virtual AWeapons* GetWeapon() override;

	UFUNCTION(BlueprintCallable)
	virtual void Unsheath() override;
};
