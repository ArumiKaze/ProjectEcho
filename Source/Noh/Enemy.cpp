#include "Enemy.h"



AEnemy::AEnemy()
{
}
AEnemy::AEnemy(int health)
	:m_health{ health }, m_invincible{ false }
{
}



void AEnemy::onWeaponHit(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}



void AEnemy::invicibleDuration()
{
	m_invincible = false;
}



void AEnemy::takeDamage(int damage)
{
	m_health -= damage;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("%d"), m_health));
}



void AEnemy::BeginPlay()
{
	Super::BeginPlay();

}



void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


