//Abstract class for Enemy//

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "INTERFACE_Enemy.h"
#include "NohCharacter.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Enemy.generated.h"

UCLASS()
class NOH_API AEnemy : public ACharacter, public IINTERFACE_Enemy
{
	GENERATED_BODY()

private:

	int m_health;



	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

protected:

	bool m_invincible;
	FTimerHandle invicibleframe_timer;


	UPROPERTY()
		class UCapsuleComponent* EnemyCapsule;

	UPROPERTY()
		class ANohCharacter* m_nohcharacter;



	virtual void onWeaponHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void takeDamage(int damage);

	void invicibleDuration();

public:	

	AEnemy();
	AEnemy(int health);
};
