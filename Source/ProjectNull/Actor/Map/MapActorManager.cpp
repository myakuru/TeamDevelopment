#include "MapActorManager.h"

#include <ProjectNull/SaveGame/MySaveGame.h>

void UMapActorManager::LoadFromSaveData(UMySaveGame* SaveData)
{
	//セーブデータを取得
	if (!SaveData)return;
	CurrentSaveData = SaveData;
}

void UMapActorManager::RegisterDestroyedActor(const FName& ActorID)
{
	if (!CurrentSaveData)return;
	CurrentSaveData->DestroyedActorIDs.Add(ActorID);
}

void UMapActorManager::RemoveDestroyedActor(const FName& ActorID)
{
	if (!CurrentSaveData)return;
	CurrentSaveData->DestroyedActorIDs.Remove(ActorID);
}

bool UMapActorManager::IsDestroyedActor(const FName& ActorID) const
{
	if (!CurrentSaveData) return false;
	return CurrentSaveData->DestroyedActorIDs.Contains(ActorID);
}
