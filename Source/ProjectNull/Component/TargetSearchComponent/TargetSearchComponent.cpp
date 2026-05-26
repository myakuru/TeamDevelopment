
#include "TargetSearchComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>


UTargetSearchComponent::UTargetSearchComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UTargetSearchComponent::BeginPlay()
{
	Super::BeginPlay();
	EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
}


void UTargetSearchComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

TArray<FEnemyDistanceData> UTargetSearchComponent::FindEnemiesSortedByDistance(float Range)
{
	TArray<FEnemyDistanceData> Result;

	if (!EnemyManager || !GetOwner()) { return Result; }

	// 座標取得
	const FVector Location = GetOwner()->GetActorLocation();

	// 範囲Sqを計算
	const float RangeSq = FMath::Square(Range);

	for (const auto& Enemy : EnemyManager->GetEnemyList())
	{
		if (!Enemy) { continue; }
		
		// 敵座標取得
		const FVector EnemyLocation = Enemy->GetActorLocation();

		// 距離Sqを計算
		const float DistSq = FVector::DistSquared(
				Location,
				EnemyLocation);

		// 範囲外だったらリストに追加しない
		if (DistSq > RangeSq) { continue; }

		// 敵の距離データに格納する
		FEnemyDistanceData Data;
		Data.Enemy		= Enemy;
		Data.DistanceSq = DistSq;
		Data.ToEnemyVector = (EnemyLocation - Location).GetSafeNormal();

		// リストに追加
		Result.Add(Data);
	}

	// 近い順ソート行う
	Result.Sort(
		[](const FEnemyDistanceData& A,
			const FEnemyDistanceData& B) {
			return A.DistanceSq < B.DistanceSq;
		});

	return Result;
}

