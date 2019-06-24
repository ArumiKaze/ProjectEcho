#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "Katana.generated.h"

UCLASS()
class NOH_API AKatana : public AWeapons
{
	GENERATED_BODY()

private:

	//---Katana and Saya blueprints---//
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKatana> bp_katana;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> bp_saya;

	//---Animation Montages---//
	//Unsheathing
	UPROPERTY()
	UAnimMontage* nbattou_idle;
	UPROPERTY()
	UAnimMontage* nbattou_moving;
	//Sheathing
	UPROPERTY()
	UAnimMontage* nkamae_noto_idle;
	UPROPERTY()
	UAnimMontage* nkamae_noto_moving;
	UPROPERTY()
	UAnimMontage* nkamae_chiburinoto_idle;
	UPROPERTY()
	UAnimMontage* nkamae_chiburinoto_moving;

protected:

	UFUNCTION(BlueprintCallable)
	virtual void weaponAction(int combophase) override;

public:

	AKatana();

	//---Debug Print---//
	UFUNCTION(BlueprintCallable)
	virtual void Debugprint() const override;

	//---Get Katana---//
	UFUNCTION(BlueprintCallable)
	virtual AWeapons* GetWeapon() override;

	//---Unsheath Katana---//
	UFUNCTION(BlueprintCallable)
	virtual void Unsheath() const override;

	//---Sheath Katana---//
	UFUNCTION(BlueprintCallable)
	virtual void Sheath() const override;
};
