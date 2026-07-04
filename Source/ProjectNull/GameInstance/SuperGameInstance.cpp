#include "SuperGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/SaveGame/MySaveGame.h>
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
#include<ProjectNull/Stage/Manager/StageManager.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/System/Result/ResultManager/ResultManager.h>
#include <ProjectNull/Actor/Map/MapActorManager.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

void USuperGameInstance::Init()
{
	//class UStageDataAssetを消したら
	//Super::InitがなぜかUObjectのInitになる
	UGameInstance::Init();

	// WeaponManager生成・初期化
	if (WeaponManagerClass) {
		WeaponManager = NewObject<UWeaponManager>(this, WeaponManagerClass);
	}
	if (WeaponManager) WeaponManager->Initialize();

	// ResultManager生成・初期化
	if (ResultManagerClass) {
		ResultManager = NewObject<UResultManager>(this, ResultManagerClass);
	}
	if (ResultManager)ResultManager->Initialize();

	//StageManager
	if (StageManagerClass) {
		StageManager = NewObject<UStageManager>(this, StageManagerClass);
	}
	if (StageManager) StageManager->Initialize();

	//MapActorManager
	if (MapActorManagerClass) {
		MapActorManager = NewObject<UMapActorManager>(this, MapActorManagerClass);
	}

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
