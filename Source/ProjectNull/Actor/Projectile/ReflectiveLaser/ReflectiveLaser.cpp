
#include "ReflectiveLaser.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

AReflectiveLaser::AReflectiveLaser():
	ReflectionInterval(0.f),
	FindDistSq(0.f),
	ReflectionCount(0)
{
}

AReflectiveLaser::AReflectiveLaser(int32 InReflectionCount):
	ReflectionInterval(0.f),
	FindDistSq(0.f),
	ReflectionCount(InReflectionCount)
{
}

void AReflectiveLaser::HandleCollision(AActor* OtherActor)
{
	if (!OtherActor || OtherActor == this || !OwnerActor) { return; }


	if (ReflectionCount <= 0)
	{
		Destroy();
		return;
	}

	ReflectionCount--;
	UE_LOG(LogTemp, Display, TEXT("ReflectionCount %d"), ReflectionCount);

	auto Enemy = Cast<AEnemyBase>(OtherActor);
	if (!Enemy) { return; }

	if (ReflectedEnemies.Contains(Enemy)) { return; }

	ReflectedEnemies.Add(Enemy);

	FVector FindLocation = OwnerActor->GetActorLocation();

	FindLocation = Enemy->GetActorLocation();

	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([this, FindLocation]()
		{
			ReflectLaserBullet(FindLocation);
		});

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }

	//CurrentProjectileMovementComp->Velocity = FVector::ZeroVector;

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

	auto TargetSearchComponent = Player->GetTargetSearchComponent();
	if (!TargetSearchComponent) { return; }

	const TArray<FEnemyDistanceData> DataArray
		= TargetSearchComponent->FindEnemiesSortedByDistance(FindDistSq, FindLocation);

	if (DataArray.IsEmpty()) {
		Destroy();
		return;
	}

	FVector ToEnemyVector = Player->GetActorForwardVector();


	for (int32 Index = 0; Index < DataArray.Num(); ++Index)
	{
		// まだ反射していない敵ならば
		if (!ReflectedEnemies.Contains(DataArray[Index].Enemy))
		{
			ToEnemyVector = DataArray[Index].ToEnemyVector;
			break;
		}
	}

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }
	SetActorLocation(FindLocation);
	CurrentProjectileMovementComp->Velocity = ToEnemyVector * 2000.f;
}

