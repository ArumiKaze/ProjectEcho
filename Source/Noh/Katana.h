#pragma once

#include "CoreMinimal.h"
#include "Weapons.h"
#include "NohCharacterEnums.h"
#include "Katana.generated.h"

UCLASS()
class NOH_API AKatana : public AWeapons
{
	GENERATED_BODY()

private:

	//---Current Katana State---//
	E_KATANASTATE katanastate;

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

public:

	AKatana();

	//---Get Katana---//
	UFUNCTION(BlueprintCallable)
	virtual AWeapons* GetWeapon(ACharacter*& nohref) override;

	//---Unsheath Katana---//
	UFUNCTION(BlueprintCallable)
	virtual void Unsheath(ACharacter*& nohref) override;

	//---Sheath Katana---//
	UFUNCTION(BlueprintCallable)
	virtual void Sheath(ACharacter*& nohref) override;
};