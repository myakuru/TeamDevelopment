#pragma once

#include "CoreMinimal.h"
#include "StageProgressData.generated.h"

/** ステージの進捗 */
USTRUCT(BlueprintType)
struct FStageProgressData
{
	GENERATED_BODY();

	/** 解放フラグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUnlocked = false;

	/** スコア */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HighScore = 0;

	/** 3つのミッションクリアフラグ */
	UPROPERTY(BlueprintReadWrite)
	TArray<bool> MissionClears;

	///** ステージクリア */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//bool StageClearAchievement = false;

	///** 中ボス出現 */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//bool MidBossAppearsAchievement = false;

	///** 中ボス撃破 */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly)
	//bool MidBossWinAchievement = false;
};

