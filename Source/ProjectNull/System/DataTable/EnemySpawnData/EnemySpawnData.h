#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnData.generated.h"

UENUM(BlueprintType)
enum class ESpawnPattern : uint8
{
	Single		UMETA(DisplayName = "Single"),			// 単体で出現
	Circle		UMETA(DisplayName = "Circle"),			// 円形に出現
	Line		UMETA(DisplayName = "Line"),			// 線状に出現
	RandomMulti UMETA(DisplayName = "RandomMulti"),		// ランダムに複数体出現
};

/// <summary>
///  Waveごとの敵の出現パターンを管理するDataTable用の構造体
/// </summary>
USTRUCT(BlueprintType)
struct FEnemySpawnUnit
{
	GENERATED_BODY()

	// 出現する敵クラス
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyClass = nullptr;

	// 出現数
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount = 1;

	// 個別パターンを使うかどうか
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverridePattern = false;

	// 個別パターン
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnPattern SpawnPattern = ESpawnPattern::Single;
};

/// <summary>
/// Waveごとの敵出現情報を管理する DataTable 用構造体
/// </summary>
USTRUCT(BlueprintType)
struct PROJECTNULL_API FEnemySpawnData : public FTableRowBase
{
	GENERATED_BODY()

	// 出現する敵一覧
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnUnit> Enemies;
};