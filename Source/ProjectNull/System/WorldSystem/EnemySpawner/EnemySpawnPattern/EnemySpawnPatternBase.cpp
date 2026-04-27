#include "EnemySpawnPatternBase.h"

TArray<FVector> UEnemySpawnPatternBase::GenerateSpawnTransforms_Implementation
(
	int32 SpawnCount,
	const FVector& OriginVector) const
{
	return {};
}

FVector UEnemySpawnPatternBase::CalculateEnemySpawnPointInRing(const FVector& Center) const
{
	// ランダム角度
	float angle = FMath::RandRange(0.0f, SpawnPosParams.kFullCircleDeg);

	// ランダム半径
	const float distance = FMath::FRandRange(SpawnPosParams.SpawnMinRadius, SpawnPosParams.SpawnMaxRadius);

	// XYオフセット
	FVector offset = {
		FMath::Cos(FMath::DegreesToRadians(angle)) * distance,
		FMath::Sin(FMath::DegreesToRadians(angle)) * distance,
		0.0f
	};

	return Center + offset;
}