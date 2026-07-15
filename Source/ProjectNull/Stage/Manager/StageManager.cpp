#include "StageManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

//ゲームインスタンス
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include <ProjectNull/SaveGame/StageProgressData.h>

#include <ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>

#include <ProjectNull/System/Result/ResultManager/ResultManager.h>
#include <ProjectNull/Data/Result/ClearRankData/ClearRankData.h>
#include <ProjectNull/Data/Result/RankConditionData/RankConditionData.h>


void UStageManager::Initialize() {
	NowStageIndex = StageDefinition::OutGameStageIndex;

	//制限時間切れでステージクリア処理を呼ぶ（一度だけバインド）
	StageTimer.OnFinished.AddUObject(this, &UStageManager::InGameFinalize);

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

		// 最初のステージは解放しておく
		if (StageCount > 0) {

			//ステージのクリア状況を初期化(デバッグ用)
			for (int i = 0; i < StageCount; i++)
			{
				(*StageProgressList)[i].MissionClears.SetNum(3);

				(*StageProgressList)[i].bUnlocked = false;
			}

			(*StageProgressList)[0].bUnlocked = true;
		}

	}

	if ((*StageProgressList)[0].MissionClears.IsEmpty()) {
		for (int i = 0; i < StageCount; i++) {
			(*StageProgressList)[i].MissionClears.SetNum(3);
		}
	}
}

void UStageManager::SaveToData(UMySaveGame* inSaveGame)
{
	inSaveGame->StageProgressList = *StageProgressList;
}

void UStageManager::InGameInitialize(int32 inNowStageIndex)
{
	NowStageIndex = inNowStageIndex;

	//取得ギアのリセット
	AcquiredWeapons.Reset();

	// クリア状況を初期化
	ResultData = FResultData();

	//マウスを隠す
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();
	if (!PC)return;
	PC->bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	PC->SetInputMode(InputMode);
	
	//BGM再生
	if (InGameBGMSound && !InGameBGMSoundComponent)
	{
		InGameBGMSoundComponent = 
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Spawn2D(InGameBGMSound,1.0f,
				1.0f,0.0f,nullptr,
				true,true);
	}
}

void UStageManager::StartStageTimer()
{
	if (!StageDataAsset)return;

	//制限時間は StageDataAsset から取得して開始
	StageTimer.StartTimer(GetWorld(), StageDataAsset->GetStageTimerLimit());
}

void UStageManager::InGameFinalize()
{
	if (NowStageIndex < StageDefinition::OutGameStageIndex)return;

	//次のステージ解放
	if(NowStageIndex + 1 < StageProgressList->Num())
	{
		(*StageProgressList)[NowStageIndex + 1].bUnlocked = true;
	}

	//セーブ
	GetWorld()->GetGameInstance<USuperGameInstance>()->SaveGameData();

	//クリア情報反映処理
	USuperGameInstance* gameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (gameInstance) {

		UResultManager* resultManager = gameInstance->GetResultManager();
		if (resultManager && !resultManager->GetSortedClearRankDatas().IsEmpty()) {

			// デバッグ用にクリア条件を満たす
			SetResultFlag(EResultFlag::ReachedFinalBoss);
			SetResultFlag(EResultFlag::ReachedMidBoss);

			// ResultDataに取得した武器のリストをセットしてResultManagerに渡す
			ResultData.RewardWeaponIDs = AcquiredWeapons;
			resultManager->SetResultData(ResultData);

			// クリア条件をチェックしてデータに反映
			TArray<FClearRankData> clearRankDatas = resultManager->GetSortedClearRankDatas();
			//sortedClearRankDatasはただのクリアも含めているため1つ飛ばす
			for (int i = 1; i < clearRankDatas.Num(); i++) { 
				(*StageProgressList)[NowStageIndex].MissionClears[i - 1] =
					(*StageProgressList)[NowStageIndex].MissionClears[i - 1] ||
					clearRankDatas[i].ConditionData->IsConditionMet(ResultData);
			}

		}
	}
	AcquiredWeapons.Reset();
	UGameplayStatics::OpenLevel(this, "ResultLevel");
	
	//BGM停止
	if (InGameBGMSoundComponent && InGameBGMSoundComponent->IsPlaying())
		InGameBGMSoundComponent->Stop();
}

void UStageManager::OutGameInitialize()
{
	UE_LOG(LogTemp, Warning, TEXT("Has BegunPlay = %d"),
	GetWorld()->HasBegunPlay());
	
	//BGM再生
	if (OutGameBGMSound && !OutGameBGMSoundComponent)
	{
		OutGameBGMSoundComponent = 
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Spawn2D(OutGameBGMSound,1.0f,
				1.0f,0.0f,nullptr,
				true,true);
		
		UE_LOG(LogTemp, Warning, TEXT("After Spawn = %p"),
			OutGameBGMSoundComponent.Get());
		c
	}
}

void UStageManager::OutGameFinalize()
{
	//BGM停止
	if (OutGameBGMSoundComponent && OutGameBGMSoundComponent->IsPlaying())OutGameBGMSoundComponent->Stop();
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
	
	//ログ
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("-  StageManager  -"));
	UE_LOG(LogTemp, Log, TEXT("NowLevelName : %s"), *LevelName.ToString());
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));

	bool isInGame = false;

	//データアセットを探索
	for (int i = 0; i < StageDataAsset->GetStageData().Num(); i++)
	{
		const FStageDataStruct& StageData = StageDataAsset->GetStageData()[i];
		//一致
		if (StageData.LevelName == LevelName)
		{
			//アウトゲームを終わらせる
			OutGameFinalize();
			
			//ステージ開始(マウスが持ってかれるぞ！！)
			InGameInitialize(i + StageDefinition::FirstStageIndex);

			isInGame = true;
		}
	}

	if (!isInGame)
	{
		//OutGameInitialize();
		//ゲーム起動時では再生出来ないため、次のtickに任せる
		LoadedWorld->GetTimerManager().SetTimerForNextTick(
			FTimerDelegate::CreateUObject(
				this,
				&UStageManager::OutGameInitialize
			)
		);
	}

	//ログ
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
	UE_LOG(LogTemp, Log, TEXT("-  StageManager  -"));
	UE_LOG(LogTemp, Log, TEXT("NowStageIndex : %d"),NowStageIndex);
	UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
}

