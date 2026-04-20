#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnData.generated.h"

class UEnemySpawnPatternBase;

USTRUCT(BlueprintType)
struct FEnemySpawnUnit
{
	GENERATED_BODY()

	// 出現させる敵のクラス（BPクラスをエディタから指定）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyClass = nullptr;

	// 何体出すか（現在はPatternMultiでのみ仕様）
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount = 1;

	// どのパターンで出すか（SingleやMultiをエディタで選択）
	// Instanced : エディタ上でオブジェクトを直接作成してプロパティを編集できる
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadWrite)
	TObjectPtr<UEnemySpawnPatternBase> SpawnPattern = nullptr;
};