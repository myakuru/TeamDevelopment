
#include "CrossLaserAttack.h"

#include <ProjectNull/Actor/Projectile/CrossLaserbeam/CrossLaserbeam.h>

UCrossLaserAttack::UCrossLaserAttack()
{
	
}

void UCrossLaserAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	UAttackBase::Initialize(Owner);

	CrossLaserbeam = GetWorld()->SpawnActor<ACrossLaserbeam>(CrossLaserbeamClass);
	if (!CrossLaserbeam) { return; }
	CrossLaserbeam->AttachToActor(
		Owner,
		FAttachmentTransformRules::KeepRelativeTransform);
	CrossLaserbeam->SetLaserEnabled(false);

}

void UCrossLaserAttack::Execute()
{
	CurrentAngle = 0.f;
	SetIsActive(true);
	if (!CrossLaserbeam) { return; }
	CrossLaserbeam->SetLaserEnabled(true);

}

void UCrossLaserAttack::Update(float DeltaTime)
{
	if (!IsActive() || !CrossLaserbeam) { return; }

	CurrentAngle += RotationSpeed * DeltaTime;

	CrossLaserbeam->SetActorRelativeRotation(
		FRotator(0.f, CurrentAngle, 0.f));
}

void UCrossLaserAttack::End()
{
	SetIsActive(false);
	if (!CrossLaserbeam) { return; }
	CrossLaserbeam->SetLaserEnabled(false);
}