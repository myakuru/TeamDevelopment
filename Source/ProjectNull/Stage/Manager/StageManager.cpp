#include "StageManager.h"
#include "Kismet/GameplayStatics.h"

//ゲームインスタンス
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include<ProjectNull/SaveGame/StageProgressData.h>

void UStageManager::Initialize()
{
}

void UStageManager::LoadFromSaveData(UMySaveGame* inSaveGame)
{
	StageProgressList = inSaveGame->StageProgressList;
}

void UStageManager::SaveToData(UMySaveGame* inSaveGame)
{
	inSaveGame->StageProgressList = StageProgressList;
}

void UStageManager::StageStart(int32 inNowStageIndex)
{
	NowStageIndex = inNowStageIndex;
}

void UStageManager::StageClear()
{
	if (NowStageIndex < 0)return;

	//次のステージ解放
	//if(NowStageIndex + 1 < StageProgressList.Num())
	//{
	//	StageProgressList[NowStageIndex + 1].bUnlocked = true;
	//}

	UGameplayStatics::OpenLevel(this, "MainHubLevel");
}

