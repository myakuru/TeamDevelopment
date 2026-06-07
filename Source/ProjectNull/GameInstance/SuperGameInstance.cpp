#include "SuperGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/SaveGame/MySaveGame.h>
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
#include<ProjectNull/Stage/Manager/StageManager.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/Actor/Map/MapActorManager.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

void USuperGameInstance::Init()
{
	Super::Init();


	if (WeaponManagerClass) {
		WeaponManager = NewObject<UWeaponManager>(this, WeaponManagerClass);
	}
	if (WeaponManager) WeaponManager->Initialize(WeaponDataTable,WeaponMaterialDataTable);

	//StageManager
	StageManager = NewObject<UStageManager>(this);

	//MapActorManagerの初期化
	MapActorManager = NewObject<UMapActorManager>(this);

	if (PlayerRuntimeData) {
		PlayerRuntimeData->Initialize();
	}

	LoadGameData();

}

void USuperGameInstance::LoadGameData()
{
	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	//セーブデータが存在するか確認し、存在すればロード、存在しなければ新規作成
	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)
		);
	}
	else
	{
		CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())
		);
	}

	// ステージ数取得
	if (StageDataAsset)
	{
		const int32 StageCount =StageDataAsset->StageData.Num();

		if (CurrentSaveData->StageProgressList.Num() < StageCount)
		{
			CurrentSaveData->StageProgressList.SetNum(StageCount);
		}

		// 最初のステージは解放しておく
		if (StageCount > 0) {

			//ステージのクリア状況を初期化(デバッグ用)
			//for(int i = 0; i < StageCount; i++)
			//{
			//	CurrentSaveData->StageProgressList[i].MissionClears.SetNum(3);

			//	CurrentSaveData->StageProgressList[i].bUnlocked = false;
			//}

			CurrentSaveData->StageProgressList[0].bUnlocked = true;
		}
	}

	//セーブデータを渡す
	if (StageManager) {
		StageManager->LoadFromSaveData(CurrentSaveData);
	}
	if (WeaponManager) {
		WeaponManager->LoadFromSaveData(CurrentSaveData);
	}
	if (MapActorManager) {
		MapActorManager->LoadFromSaveData(CurrentSaveData);
	}
}

void USuperGameInstance::SaveGameData()
{
	if (!CurrentSaveData)return;

	UE_LOG(LogTemp, Error, TEXT("Save now! : CurrentSaveData"));

	//ステージマネージャー
	if (StageManager) {
		StageManager->SaveToData(CurrentSaveData);
	}

	if (WeaponManager) {
		WeaponManager->SaveToData(CurrentSaveData);
	}

	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	UGameplayStatics::SaveGameToSlot(CurrentSaveData, SlotName, UserIndex);
}

inline void USuperGameInstance::SetStageScore(int32 inStageIndex, int32 inScore)
{
	if (!CurrentSaveData || 
		!CurrentSaveData->StageProgressList.IsValidIndex(inStageIndex)) return;

	if(CurrentSaveData->StageProgressList[inStageIndex].HighScore < inScore)
		CurrentSaveData->StageProgressList[inStageIndex].HighScore = inScore;
}
