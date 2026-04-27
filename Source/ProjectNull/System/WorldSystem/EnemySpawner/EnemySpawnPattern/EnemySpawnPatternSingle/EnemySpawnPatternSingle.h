#pragma once

#include "CoreMinimal.h"
#include "../EnemySpawnPatternBase.h"
#include "EnemySpawnPatternSingle.generated.h"

// OriginTransform の１点だけに出す
// SpawnCountは無視（常に１体）

UCLASS(EditInlineNew)
class PROJECTNULL_API UEnemySpawnPatternSingle : public UEnemySpawnPatternBase
{
	GENERATED_BODY()

public:

	virtual TArray<FVector> GenerateSpawnTransforms_Implementation(
		int32 SpawnCount,
		const FVector& OriginTransform) const override;

};