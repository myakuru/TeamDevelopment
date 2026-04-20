#include "EnemySpawnPatternSingle.h"

TArray<FVector> UEnemySpawnPatternSingle::GenerateSpawnTransforms_Implementation
(
	int32 SpawnCount, 
	const FVector& OriginVector) const
{
	TArray<FVector> Results;
	// OriginTransform‚ð‚»‚Ì‚Ü‚Ü‚P‚Â‚¾‚¯’Ç‰Á‚µ‚Ä•Ô‚·
	Results.Add(OriginVector);
	return Results;
}