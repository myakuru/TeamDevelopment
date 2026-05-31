
#include "LaserBulletShooter.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>



ULaserBulletShooter::ULaserBulletShooter() :
	Owner(nullptr),
	Player(nullptr),
	ProjectileClass(nullptr),
	StartLocation(FVector::ZeroVector),
	StartVelocityArray(TArray<FVector>()),
	TargetArray(TArray<TWeakObjectPtr<AEnemyBase>>()),
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
	Owner	= InOwner;
	Player	= Cast<APlayerBase>(InOwner);
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
		&ULaserBulletShooter::ShotLaserBulletAndIncrementCount,
		ShotInterval,
		true);


	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies
		= TargetSearch->FindEnemiesSortedByDistance(TargetableDistSq,InStartLocation);

	InitVelocityArray(Player->GetActorForwardVector());

	//※発射する瞬間に座標やベクトル知りたい

	/*for (int32 Index = 0; Index < BulletNum && Index < Enemies.Num(); ++Index)
	{
		StartVelocityArray[Index] = Enemies[Index].ToEnemyVector;
	}*/
	TargetArray.SetNum(BulletNum);

	for (int32 Index = 0; Index < BulletNum && Index < Enemies.Num(); ++Index)
	{
		TargetArray[Index] = Enemies[Index].Enemy;
	}

	ShotLaserBullet();
}

void ULaserBulletShooter::ShotLaserBulletAndIncrementCount()
{
	//※インデックスで判定、命名の修正
	if (ShotCount + 1 >= BulletNum) { return; }
	ShotCount++;

	UE_LOG(LogTemp, Display, TEXT("Add処理 cnt %d"), ShotCount);
	ShotLaserBullet();
}

void ULaserBulletShooter::ShotLaserBullet()
{
	auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(ProjectileClass);
	if (!Owner) { return; }
	if (!LaserBullet) { return; }

	const FVector Location = Owner->GetActorLocation();

	LaserBullet->SetOwnerActor(Owner);
	LaserBullet->SetActorLocation(Location);

	FVector TargetLocation = FVector::ZeroVector;
	FVector StartVelocity = Owner->GetActorForwardVector();


	if (TargetArray.IsValidIndex(ShotCount) && TargetArray[ShotCount].IsValid())
	{
		const auto Target = TargetArray[ShotCount].Get();
		TargetLocation = Target->GetActorLocation();
		StartVelocity = (Target->GetActorLocation() - Location).GetSafeNormal();
	}

	LaserBullet->SetTargetLocation(TargetLocation);

	auto ProjectileMovement = LaserBullet->GetProjectileMovement();

	if (!ProjectileMovement)	{ return; }
	

	/*if (StartVelocityArray.IsValidIndex(ShotCount))
	{
		StartVelocity = StartVelocityArray[ShotCount];
	}*/



	ProjectileMovement->Velocity = StartVelocity * BulletSpeed;
}
