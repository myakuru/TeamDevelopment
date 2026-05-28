
#include "LaserGearStateBase.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

ULaserGearStateBase::ULaserGearStateBase():
	bDrawDebugLine(false)
{
}

void ULaserGearStateBase::ShotTargetedLaserBullets(FLaserBulletData& InBulletData)
{
	if (!Player) { return; }


	GetWorld()->GetTimerManager().SetTimer(
		InBulletData.ShotIntervalTimerHandle,
		this,
		&ULaserGearStateBase::,
		InBulletData.ShotInterval,
		true);


	const FVector SpawnLocation = Player->GetActorLocation();

	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies
		= TargetSearch->FindEnemiesSortedByDistance(InBulletData.TargetableDistSq);

	InBulletData.InitVelocityArray(Player->GetActorForwardVector());

	for (int32 Index = 0; Index < InBulletData.Num && Index < Enemies.Num(); ++Index)
	{
		InBulletData.StartVelocityArray[Index] = Enemies[Index].ToEnemyVector;
	}

	

}

void ULaserGearStateBase::ShotLaserBullet(
	const FLaserBulletData& InBulletData,
	const FVector& InSpawnLocation)
{


	auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(
		InBulletData.Class);

	if (!LaserBullet) { return; }

	LaserBullet->SetOwnerActor(Player);
	LaserBullet->SetActorLocation(InSpawnLocation);

	auto ProjectileMovement = LaserBullet->GetProjectileMovement();

	if (!ProjectileMovement) { return; }

	ProjectileMovement->Velocity = InBulletData. * InBulletData.Speed;
}
