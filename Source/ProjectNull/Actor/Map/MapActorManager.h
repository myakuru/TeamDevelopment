#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "MapActorManager.generated.h"

class UMySaveGame;

/**
 *  MapActorを管理するマネージャー
 */
UCLASS()
class PROJECTNULL_API UMapActorManager : public UObject
{
	GENERATED_BODY()
	
public:

	void LoadFromSaveData(UMySaveGame* SaveData);

	/** 破壊するオブジェクトのIDを保存 */
	void RegisterDestroyedActor(const FName& ActorID);

	void RemoveDestroyedActor(const FName& ActorID);

	/** 破壊オブジェクトとして保存されているか */
	bool IsDestroyedActor(const FName& ActorID) const;

private:

	UPROPERTY()
	UMySaveGame* CurrentSaveData;
};
