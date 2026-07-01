
#include "ReflectiveLaser.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

AReflectiveLaser::AReflectiveLaser():
	BulletSpeed(0.f),
	ReflectionInterval(0.f),
	FindDistSq(0.f),
	ReflectionCount(0),
	ReflectedEnemies(TSet<TWeakObjectPtr<AEnemyBase>>()),
	ReflectionIntervalTimerHandle(FTimerHandle())
{
}

AReflectiveLaser::AReflectiveLaser(int32 InReflectionCount):
	BulletSpeed(0.f),
	ReflectionInterval(0.f),
	FindDistSq(0.f),
	ReflectionCount(InReflectionCount),
	ReflectedEnemies(TSet<TWeakObjectPtr<AEnemyBase>>()),
	ReflectionIntervalTimerHandle(FTimerHandle())
{
}

void AReflectiveLaser::HandleCollision(AActor* OtherActor)
{
	if (!OtherActor || 
		OtherActor == this || 
		!OwnerActor) { return; }

	
	auto Enemy = Cast<AEnemyBase>(OtherActor);
	if (!Enemy) { return; }

	if (ReflectedEnemies.Contains(Enemy)) { return; }

	if (ReflectionCount <= 0)
	{
		Destroy();
		return;
	}


	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	{
		Interface->ApplyDamaged();
		Interface->ApplyKnockBack(OwnerActor->GetActorLocation());
	}

	ReflectionCount--;
	//UE_LOG(LogTemp, Display, TEXT("ReflectionCount %d"), ReflectionCount);

	ReflectedEnemies.Add(Enemy);

	const FVector TargetLocation = Enemy->GetActorLocation();

	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([this, TargetLocation]()
		{
			ReflectLaserBullet(TargetLocation);
		});

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }

	CurrentProjectileMovementComp->Velocity = FVector::ZeroVector;

	SetActorLocation(TargetLocation);

	GetWorld()->GetTimerManager().SetTimer(
		ReflectionIntervalTimerHandle,
		TimerDelegate,
		ReflectionInterval,
		false);
}

void AReflectiveLaser::ReflectLaserBullet(const FVector& FindLocation)
{
	auto Player = Cast<APlayerBase>(OwnerActor);
	if (!Player) { return; }

	auto TargetSearchComp = Player->GetTargetSearchComponent();
	if (!TargetSearchComp) { return; }

	const TArray<FEnemyDistanceData> DataArray
		= TargetSearchComp->FindEnemiesSortedByDistance(FindDistSq, FindLocation);

	if (DataArray.IsEmpty()) 
	{
		Destroy();
		return;
	}

	FVector ToEnemyVector = Player->GetActorForwardVector();

	bool bEnd = true;

	for (int32 Index = 0; Index < DataArray.Num(); ++Index)
	{
		// まだ反射していない敵ならば
		if (!ReflectedEnemies.Contains(DataArray[Index].Enemy))
		{
			ToEnemyVector = DataArray[Index].ToEnemyVector;
			bEnd = false;
			break;
		}
	}

	if (bEnd) {
		Destroy();
		return;
	}

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }

	CurrentProjectileMovementComp->Velocity = ToEnemyVector * BulletSpeed;
}


