
#include "ReflectiveLaser.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

AReflectiveLaser::AReflectiveLaser():
	ReflectionInterval(0.f),
	ReflectionCount(0)
{
}

AReflectiveLaser::AReflectiveLaser(int32 InReflectionCount):
	ReflectionInterval(0.f),
	ReflectionCount(InReflectionCount)
{
}

void AReflectiveLaser::OnCollisionOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AProjectileBase::OnCollisionOverlap(
		OverlappedComponent,
		OtherActor,
		OtherComp,
		OtherBodyIndex,
		bFromSweep,
		SweepResult);


	GetWorld()->GetTimerManager().SetTimer(
		ReflectionIntervalTimerHandle,
		this,
		&AReflectiveLaser::ReflectLaserBullet,
		ReflectionInterval,
		false);
}

void AReflectiveLaser::ReflectLaserBullet()
{
	auto Player = Cast<APlayerBase>(OwnerActor);
	if (!Player) { return; }

	auto TargetSearchComponent = Player->GetTargetSearchComponent();
	if (!TargetSearchComponent) { return; }

	const TArray<FEnemyDistanceData> DataArray
		= TargetSearchComponent->FindEnemiesSortedByDistance(2000.f);
	if (!DataArray.IsValidIndex(0)) { return; }

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }

	CurrentProjectileMovementComp->Velocity = DataArray[0].ToEnemyVector * 2000.f;

}
