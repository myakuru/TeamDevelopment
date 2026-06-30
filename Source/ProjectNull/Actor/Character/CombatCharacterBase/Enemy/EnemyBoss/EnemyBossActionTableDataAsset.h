#pragma once

#include "Engine/DataAsset.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossActionTableDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FBossActionEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	EBossActionType ActionType = EBossActionType::ApproachWalk;

	// 相対的な重み（例: 3なら他の2倍出やすい）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ClampMin = "0.0"))
	float Weight = 1.0f;
};

UCLASS(BlueprintType)
class PROJECTNULL_API UBossActionTableDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	// 近距離（Dist <= NearRange）時の行動テーブル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	TArray<FBossActionEntry> NearActions;

	// 遠距離時の行動テーブル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Action")
	TArray<FBossActionEntry> FarActions;
};
