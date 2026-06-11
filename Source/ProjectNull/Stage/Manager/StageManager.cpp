#include "StageManager.h"
#include "Kismet/GameplayStatics.h"

//ゲームインスタンス
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include <ProjectNull/SaveGame/StageProgressData.h>

#include <ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>

void UStageManager::Initialize() {

	NowStageIndex = StageDefinition::OutGameStageIndex;

	if (!StageDataAsset)return;

	//ステージを調査
	ChangeStageInvestigation(GetWorld());

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(
		this,
		&UStageManager::ChangeStageInvestigation
	);
}

void UStageManager::LoadFromSaveData(UMySaveGame* inSaveGame)
{
	StageProgressList = &inSaveGame->StageProgressList;

	// ステージ数取得
	if (!StageDataAsset)return;
	const int32 StageCount = StageDataAsset->GetStageData().Num();

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

void UStageManager::SaveToData(UMySaveGame* inSaveGame)
{
	inSaveGame->StageProgressList = *StageProgressList;
}

void UStageManager::StageStart(int32 inNowStageIndex)
{
	NowStageIndex = inNowStageIndex;

	//取得ギアのリセット
	AcquiredWeapons.Reset();

	//マウスを隠す
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();
	if (!PC)return;
	PC->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
}

void UStageManager::StageClear()
{
	if (NowStageIndex < StageDefinition::OutGameStageIndex)return;

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

void UStageManager::ChangeStageInvestigation(UWorld* LoadedWorld)
{
	//レベルの名前が設定されているステージのレベル名と一致するか

	NowStageIndex = StageDefinition::OutGameStageIndex;

	//レベルの名前
	FName LevelName = FName(
		*UGameplayStatics::GetCurrentLevelName(
			LoadedWorld,
			true
		));

	//データアセットを探索
	for (int i = 0; i < StageDataAsset->GetStageData().Num(); i++)
	{
		const FStageDataStruct& StageData = StageDataAsset->GetStageData()[i];
		//一致
		if (StageData.LevelName == LevelName)
		{
			//ステージ開始(マウスが持ってかれるぞ！！)
			StageStart(i + StageDefinition::FirstStageIndex);
		}
	}

	//ログ
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("-  StageManager  -"));
	UE_LOG(LogTemp, Log, TEXT("NowLevelName : %s"), *LevelName.ToString());
	UE_LOG(LogTemp, Log, TEXT("NowStageIndex : %d"),NowStageIndex);
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
}

