#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include  "ProjectNull/System/DataTable/EnemySpawnData/EnemySpawnData.h"
#include "EnemyWaveDataAsset.generated.h"

// ウェーブ１回分のデータをまとめたDataAsset
// Enemies配列にFEnemySpawnUnitを並べて
// このウェーブでは○○を△体、□□パターンでという感じで使う

UCLASS(BlueprintType)
class PROJECTNULL_API UEnemyWaveDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/**
	* このウェーブで出現しうる敵のリスト
	* 毎スポーン時にCreateProbabilityを重みとして
	* ランダムに1種類が選ばれる
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FEnemySpawnUnit> Enemies;

};