
#include "CrossLaserAttack.h"

UCrossLaserAttack::UCrossLaserAttack()
{
}

void UCrossLaserAttack::Execute()
{
	CurrentAngle = 0.f;
	bIsActive = true;
}

void UCrossLaserAttack::Update(
	float DeltaTime,
	AActor* Player,
	UEnemyManagerSubsystem* EnemyManager)
{
	if (!bIsActive) { return; }

	CurrentAngle += RotationSpeed * DeltaTime;
}

void UCrossLaserAttack::End()
{
	bIsActive = false;
}