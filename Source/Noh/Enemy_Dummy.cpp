#include "Enemy_Dummy.h"



AEnemy_Dummy::AEnemy_Dummy()
	:AEnemy(1000)
{
	PrimaryActorTick.bCanEverTick = true;

	EnemyCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("enemydummy_capsule"));
	EnemyCapsule->InitCapsuleSize(55.f, 96.0f);;
	EnemyCapsule->SetCollisionProfileName(TEXT("Hit"));
	EnemyCapsule->SetupAttachment(RootComponent);
	EnemyCapsule->OnComponentBeginOverlap.AddDynamic(this, &AEnemy_Dummy::onWeaponHit);
}



void AEnemy_Dummy::onWeaponHit(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	m_nohcharacter = Cast<ANohCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (m_nohcharacter)
	{
		if (OtherActor == m_nohcharacter->getWeapon() && !m_invincible && m_nohcharacter->getIsAttacking())
		{
			m_invincible = true;
			takeDamage(m_nohcharacter->getWeaponDamage());
			GetWorld()->GetTimerManager().SetTimer(invicibleframe_timer, this, &AEnemy_Dummy::invicibleDuration, 1.0f);
		}
	}
}