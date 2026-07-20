#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemySpawnPattern/EnemyWaveDataAsset.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyMidBossBase/EnemyMidBossBase.h>
#include "EnemyPhaseSpawnTable.generated.h"

// フェーズとWaveDataAssetの対応を1セットにまとめた構造体
USTRUCT()
struct FPhaseSpawnWave
{
    GENERATED_BODY()

    // 対応するフェーズ番号
    UPROPERTY(EditAnywhere)
    int32 Phase = 1;

    // 各フェーズで生成するエネミーの数
    UPROPERTY(EditAnywhere)
    int32 PhaseCreateEnemyNum = 1;

	// フェーズの上昇に必要な倒した敵の数
	UPROPERTY(EditAnywhere)
	int32 PhaseUpDeathEnemyCount = 0;

	// 出現インターバル
	UPROPERTY(EditAnywhere)
	float PhaseInterval = 2.0f;

    // そのフェーズで使うフェーズデータ
    UPROPERTY(EditAnywhere)
    TObjectPtr<UEnemyWaveDataAsset> WaveData = nullptr;
};

UCLASS()
class PROJECTNULL_API UEnemyPhaseSpawnTable : public UDataAsset
{
    GENERATED_BODY()

public:

    // フェーズデータの配列
    UPROPERTY(EditAnywhere)
    TArray<FPhaseSpawnWave> PhaseWaves;

	// 最終ウェーブの設定
	UPROPERTY(EditAnywhere)
	int FinalWave = 0;

	// ボス専用ウェーブの設定
	UPROPERTY(EditAnywhere)
	int BossWave = 0;

	// 中ボス
	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyMidBossBase> StageTyuuBoss;

	// 出現するボス
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AEnemyBossBase> StageBoss;

    // フェーズに対応したデータアセットを探す
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

    // フェーズに対応したエネミー数を返す
    const int32 FindEnemyNumByPhase(int32 InPhase) const
    {
        for (const FPhaseSpawnWave& Row : PhaseWaves)
        {
            if (Row.Phase == InPhase)
            {
                return Row.PhaseCreateEnemyNum;
            }
        }
        return 0;
    }
};