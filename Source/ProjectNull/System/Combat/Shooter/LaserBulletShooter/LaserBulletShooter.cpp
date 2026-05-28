
#include "LaserBulletShooter.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

ULaserBulletShooter::ULaserBulletShooter() :
	Owner(nullptr),
	Class(nullptr),
	TargetableDistSq(0.0f),
	Num(1),
	Speed(0.f),
	ShotInterval(0.f),
	ShotCount(1)
{
}

void ULaserBulletShooter::ShotTargetedLaserBullets(const FVector& InSpawnLocation)
{
	if (!Owner) { return; }


	/*GetWorld()->GetTimerManager().SetTimer(
		InBulletData.ShotIntervalTimerHandle,
		this,
		&ULaserGearStateBase::,
		InBulletData.ShotInterval,
		true);*/


	const FVector SpawnLocation = Player->GetActorLocation();

	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies
		= TargetSearch->FindEnemiesSortedByDistance(TargetableDistSq);

	InitVelocityArray(Player->GetActorForwardVector());

	for (int32 Index = 0; Index < Num && Index < Enemies.Num(); ++Index)
	{
		StartVelocityArray[Index] = Enemies[Index].ToEnemyVector;
	}



}

void ULaserBulletShooter::Update(float DeltaTime)
{


}

void ULaserBulletShooter::ShotLaserBullet(const FVector& InSpawnLocation)
{
	auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(Class);

	if (!LaserBullet) { return; }

	LaserBullet->SetOwnerActor(Owner);
	LaserBullet->SetActorLocation(InSpawnLocation);

	auto ProjectileMovement = LaserBullet->GetProjectileMovement();

	if (!ProjectileMovement) { return; }

	ProjectileMovement->Velocity = StartVelocityArray[ShotCount] * Speed;
}

