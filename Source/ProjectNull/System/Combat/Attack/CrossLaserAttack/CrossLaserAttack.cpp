
#include "CrossLaserAttack.h"

#include <ProjectNull/Actor/Projectile/CrossLaserbeam/CrossLaserbeam.h>

UCrossLaserAttack::UCrossLaserAttack()
{
	
}

void UCrossLaserAttack::Initialize(class AActor* Owner)
{
	UAttackBase::Initialize(Owner);

	CrossLaserbeam = GetWorld()->SpawnActor<ACrossLaserbeam>(CrossLaserbeamClass);

	CrossLaserbeam->AttachToActor(
		Owner,
		FAttachmentTransformRules::KeepRelativeTransform);
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
	if (!bIsActive || !CrossLaserbeam) { return; }

	CurrentAngle += RotationSpeed * DeltaTime;

	CrossLaserbeam->SetActorRelativeRotation(
		FRotator(0.f, CurrentAngle, 0.f));
}

void UCrossLaserAttack::End()
{
	bIsActive = false;
}