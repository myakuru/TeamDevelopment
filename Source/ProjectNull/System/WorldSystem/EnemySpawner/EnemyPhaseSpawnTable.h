#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemySpawnPattern/EnemyWaveDataAsset.h"
#include "EnemyPhaseSpawnTable.generated.h"

// フェーズとWaveDataAssetの対応を1セットにまとめた構造体
USTRUCT()
struct FPhaseSpawnWave
{
    GENERATED_BODY()

    // 対応するフェーズ番号
    UPROPERTY(EditAnywhere)
    int32 Phase = 1;

    // そのフェーズで使うフェーズデータ
    UPROPERTY(EditAnywhere)
    TObjectPtr<UEnemyWaveDataAsset> WaveData = nullptr;
};

UCLASS()
class PROJECTNULL_API UEnemyPhaseSpawnTable : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere)
    TArray<FPhaseSpawnWave> PhaseWaves;

    const UEnemyWaveDataAsset* FindWaveDataByPhase(int32 InPhase) const
    {
        for (const FPhaseSpawnWave& Row : PhaseWaves)
        {
            if (Row.Phase == InPhase)
            {
                return Row.WaveData;
            }
        }
        return nullptr;
    }
};