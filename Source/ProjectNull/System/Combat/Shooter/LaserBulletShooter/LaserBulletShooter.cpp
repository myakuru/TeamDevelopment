
#include "LaserBulletShooter.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>



ULaserBulletShooter::ULaserBulletShooter() :
	Owner(nullptr),
	Player(nullptr),
	ProjectileClass(nullptr),
	StartLocation(FVector::ZeroVector),
	StartVelocityArray(TArray<FVector>()),
	TargetableDistSq(0.0f),
	BulletNum(1),
	BulletSpeed(0.f),
	ShotInterval(0.f),
	ShotCount(0),
	ShotIntervalTimerHandle(FTimerHandle())
{
}

void ULaserBulletShooter::Initialize(AActor* InOwner)
{
	Owner = InOwner;
	Player = Cast<APlayerBase>(InOwner);
}

void ULaserBulletShooter::Reset()
{
	GetWorld()->GetTimerManager().ClearTimer(ShotIntervalTimerHandle);
	ShotCount = 0;
}

void ULaserBulletShooter::ShotTargetedLaserBullets(const FVector& InStartLocation)
{
	if (!Owner || !Player) { return; }

	StartLocation = InStartLocation;

	GetWorld()->GetTimerManager().SetTimer(
		ShotIntervalTimerHandle,
		this,
		&ULaserBulletShooter::ShotLaserBullet,
		ShotInterval,
		true);


	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies
		= TargetSearch->FindEnemiesSortedByDistance(TargetableDistSq,InStartLocation);

	InitVelocityArray(Player->GetActorForwardVector());

	for (int32 Index = 0; Index < BulletNum && Index < Enemies.Num(); ++Index)
	{
		StartVelocityArray[Index] = Enemies[Index].ToEnemyVector;
	}

	ShotLaserBullet();


}

void ULaserBulletShooter::ShotLaserBullet()
{
	if (ShotCount >= BulletNum) { return; }

	ShotCount++;
	UE_LOG(LogTemp, Display, TEXT("Add処理 cnt %d"), ShotCount);

	auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass);

	if (!LaserBullet) { return; }

	LaserBullet->SetOwnerActor(Owner);
	LaserBullet->SetActorLocation(StartLocation);

	auto ProjectileMovement = LaserBullet->GetProjectileMovement();

	if (!ProjectileMovement) { return; }

	if (!Owner) { return; }

	FVector StartVelocity = Owner->GetActorForwardVector();

	if (StartVelocityArray.IsValidIndex(ShotCount))
	{
		StartVelocity = StartVelocityArray[ShotCount];
	}

	ProjectileMovement->Velocity = StartVelocity * BulletSpeed;
}
