#include "StageManager.h"
#include "Kismet/GameplayStatics.h"

//ゲームインスタンス
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include<ProjectNull/SaveGame/StageProgressData.h>

#include <ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>

void UStageManager::LoadFromSaveData(UMySaveGame* inSaveGame)
{
	StageProgressList = &inSaveGame->StageProgressList;

	// ステージ数取得
	if (StageDataAsset)
	{
		const int32 StageCount = StageDataAsset->StageData.Num();

		if (StageProgressList->Num() < StageCount)
		{
			StageProgressList->SetNum(StageCount);
		}

		// 最初のステージは解放しておく
		if (StageCount > 0) {

			//ステージのクリア状況を初期化(デバッグ用)
			//for(int i = 0; i < StageCount; i++)
			//{
			//	CurrentSaveData->StageProgressList[i].MissionClears.SetNum(3);

			//	CurrentSaveData->StageProgressList[i].bUnlocked = false;
			//}

			(*StageProgressList)[0].bUnlocked = true;
		}
	}
}

void UStageManager::SaveToData(UMySaveGame* inSaveGame)
{
	inSaveGame->StageProgressList = *StageProgressList;
}

void UStageManager::StageStart(int32 inNowStageIndex)
{
	NowStageIndex = inNowStageIndex;

	//取得ギアのリセット
	AcquiredWeapons.Reset();
}

void UStageManager::StageClear()
{
	if (NowStageIndex < 0)return;

	//次のステージ解放
	if(NowStageIndex + 1 < StageProgressList->Num())
	{
		(*StageProgressList)[NowStageIndex + 1].bUnlocked = true;
	}

	//セーブ
	GetWorld()->GetGameInstance<USuperGameInstance>()->SaveGameData();

	//取得ギアのリセット
	AcquiredWeapons.Reset();

	UGameplayStatics::OpenLevel(this, "StageSelectLevel");
}

