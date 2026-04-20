#pragma once

#include "CoreMinimal.h"
#include "../EnemySpawnPatternBase.h"
#include "EnemySpawnPatternRandomMulti.generated.h"

// OriginTransformを中心とした範囲に
// SpawnCount体分のランダム座標を生成する

UCLASS(EditInlineNew)
class PROJECTNULL_API UEnemySpawnPatternRandomMulti : public UEnemySpawnPatternBase
{
	GENERATED_BODY()

public:

	// ランダムな散らばりの最大幅
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Spawn")
	FVector RandomExtent = FVector(300.0f, 300.0f, 0.0f);

	virtual TArray<FVector> GenerateSpawnTransforms_Implementation(
		int32 SpawnCount,
		const FVector& OriginTransform) const override;

};