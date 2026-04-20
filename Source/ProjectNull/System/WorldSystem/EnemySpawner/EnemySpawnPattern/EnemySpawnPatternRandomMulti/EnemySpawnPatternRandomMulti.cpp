#include "EnemySpawnPatternRandomMulti.h"

TArray<FVector> UEnemySpawnPatternRandomMulti::GenerateSpawnTransforms_Implementation
(
	int32 SpawnCount,
	const FVector& OriginVector) const
{
	TArray<FVector> Results;

	// SpawnCount ‚ª‚OˆÈ‰º‚Å‚àÅ’á‚P‘Ì‚Ío‚·
	const int32		Count = FMath::Max(SpawnCount, 1);
	const FVector		Origin = OriginVector;

	for (int32 i = 0; i < Count; ++i)
	{
		FVector SpawnVector = CalculateEnemySpawnPointInRing(Origin);

		Results.Add(SpawnVector);
	}

	return Results;
}