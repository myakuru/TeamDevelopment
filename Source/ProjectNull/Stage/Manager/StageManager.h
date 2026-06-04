#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <ProjectNull/SaveGame/StageProgressData.h>
#include "StageManager.generated.h"

class UMySaveGame;

/**
 * ステージの状況を管理するマネージャー
 */
UCLASS()
class PROJECTNULL_API UStageManager : public UObject
{
	GENERATED_BODY()
	
public:

	void LoadFromSaveData(UMySaveGame* inSaveGame);

	void SaveToData(UMySaveGame* inSaveGame);

	UFUNCTION()
	void StageStart(int32 inNowStageIndex);

	UFUNCTION()
	void StageClear();

private:

	/** ステージの進行状況リスト */
	TArray<FStageProgressData>* StageProgressList;

	/** 現在のステージのインデックス */
	int32 NowStageIndex;
};
