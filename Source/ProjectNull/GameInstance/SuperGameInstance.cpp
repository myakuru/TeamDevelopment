#include "SuperGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/SaveGame/MySaveGame.h>
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/Actor/Map/MapActorManager.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

void USuperGameInstance::Init()
{
	Super::Init();

	WeaponManager = NewObject<UWeaponManager>(this);
	if (WeaponManager) WeaponManager->Initialize(WeaponDataTable,WeaponMaterialDataTable);

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

		CurrentSaveData->StageProgressList.SetNum(StageCount);

		// 最初のステージは解放しておく
		if (StageCount > 0) {

			for(int i = 0; i < StageCount; i++)
			{
				CurrentSaveData->StageProgressList[i].MissionClears.SetNum(3);

				CurrentSaveData->StageProgressList[i].bUnlocked = false;
			}

			CurrentSaveData->StageProgressList[0].bUnlocked = true;
		}
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
