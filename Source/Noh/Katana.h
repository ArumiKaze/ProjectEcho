#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NohCharacterEnums.h"
#include "Katana.generated.h"

UCLASS()
class NOH_API AKatana : public AActor
{
	GENERATED_BODY()

private:

	//---Katana and Saya blueprints---//
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AKatana> bp_katana;
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AActor> bp_saya;

	//---Current Katana State---//
	E_KATANASTATE m_katanastate;
	bool b_raisingsword;

	//---Animation Montages---//
	//Unsheathing
	UPROPERTY()
	UAnimMontage* animmontage_battou_idle;
	UPROPERTY()
	UAnimMontage* animmontage_battou_moving;
	//Sheathing
	UPROPERTY()
	UAnimMontage* animmontage_kamae_noto_idle;
	UPROPERTY()
	UAnimMontage* animmontage_kamae_noto_moving;
	UPROPERTY()
	UAnimMontage* animmontage_kamae_chiburinoto_idle;
	UPROPERTY()
	UAnimMontage* animmontage_kamae_chiburinoto_moving;
	//Attacks
	UPROPERTY()
	UAnimMontage* animmontage_shomengiri;

protected:

	//---Katana Mesh---//
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* mesh_weapon;

public:

	AKatana();

	//---Get Katana---//
	AKatana* GetWeapon(class ANohCharacter*& nohref);

	//---Get Mesh---//
	UStaticMeshComponent* GetKatanaMeshRef();

	//---Get Katanta State---//
	E_KATANASTATE GetKatanaState();

	//---Set Katana State---//
	void SetRaiseKatanaFinish(class ANohCharacter*& nohref);

	//---Unsheath Katana---//
	void Unsheath(class ANohCharacter*& nohref);

	//---Sheath Katana---//
	void Sheath(class ANohCharacter*& nohref);

	//---Ready Katana---//
	void ReadySkill(bool ready);

	//---Choose Attack---//
	void ChooseAttackSkill(class ANohCharacter*& nohref, float direction);
};