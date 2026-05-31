
#include "ReflectiveLaser.h"

#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

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

void AReflectiveLaser::Tick(float DeltaTime)
{
	AProjectileBase::Tick(DeltaTime);

	//UpdateSphereCollision();
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
	//UE_LOG(LogTemp, Display, TEXT("ReflectionCount %d"), ReflectionCount);

	auto Enemy = Cast<AEnemyBase>(OtherActor);
	if (!Enemy) { return; }

	if (ReflectedEnemies.Contains(Enemy)) { return; }

	ReflectedEnemies.Add(Enemy);

	FVector FindLocation = Enemy->GetActorLocation();

	FTimerDelegate TimerDelegate;

	TimerDelegate.BindLambda([this, FindLocation]()
		{
			ReflectLaserBullet(FindLocation);
		});

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }

	CurrentProjectileMovementComp->Velocity = FVector::ZeroVector;
	SetActorLocation(FindLocation);
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

	bool bEnd = true;

	for (int32 Index = 0; Index < DataArray.Num(); ++Index)
	{
		// まだ反射していない敵ならば
		if (!ReflectedEnemies.Contains(DataArray[Index].Enemy))
		{
			ToEnemyVector = DataArray[Index].ToEnemyVector;
			TargetActor = DataArray[Index].Enemy;
			bEnd = false;
			break;
		}
	}

	if (bEnd) {
		Destroy();
		return;
	}

	if (auto Target = TargetActor.Get()) {
		TargetLocation = Target->GetActorLocation();
	}

	auto CurrentProjectileMovementComp = GetProjectileMovement();
	if (!CurrentProjectileMovementComp) { return; }
	//SetActorLocation(FindLocation);
	CurrentProjectileMovementComp->Velocity = ToEnemyVector * 2000.f;

}

void AReflectiveLaser::UpdateSphereCollision()
{

	auto Target = TargetActor.Get();
	if (!Target) { return; }

	auto ProjectileMovementComp = GetProjectileMovement();
	if (!ProjectileMovementComp) { return; }
	FVector TargetActorLocation = Target->GetActorLocation();

	FVector TargetToVector = TargetLocation - TargetActorLocation;

	// 次フレームでこりじょんがすり抜けるかどうか
	if (TargetToVector.Size() >= ProjectileMovementComp->Velocity.Size()) { return; }

	auto Sphere = GetSphereCollision();
	if (!Sphere) { return; }

	const float RangeSq = FMath::Square(Sphere->GetUnscaledSphereRadius());

	if (TargetToVector.SizeSquared() <= RangeSq)
	{
		ProjectileMovementComp->Velocity = TargetToVector;
		SetActorLocation(TargetLocation);

	}
}

