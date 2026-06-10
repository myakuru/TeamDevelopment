#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SuperGameInstance.generated.h"

class UMySaveGame;
class UStageDataAsset;
class UStageManager;
class UWeaponManager;
class UMapActorManager;
class UPlayerParameterData;
class UPlayerRuntimeData;

/**
 * ゲーム全体で共有されるデータや機能を管理するクラス
 */
UCLASS()
class PROJECTNULL_API USuperGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init()override;

	UFUNCTION(BlueprintCallable)
	void LoadGameData();

	UFUNCTION(BlueprintCallable)
	void SaveGameData();

	/** セーブデータを取得する */
	inline UMySaveGame* GetCurrentSaveData() const { return CurrentSaveData; }

	/** ステージデータを取得する */
	inline TObjectPtr<UStageDataAsset> GetStageDataAsset() const { return StageDataAsset; }

	/** ステージマネージャーを取得する */
	inline TObjectPtr<UStageManager> GetStageManagerSubsystem() const { return StageManager; }

	/** 最高スコアか判定し、保存する */
	inline void SetStageScore(int32 inStageIndex, int32 inScore);

	/** パラメーターを取得する */
	inline TObjectPtr<UPlayerParameterData> GetPlayerParameterData() const { return PlayerParameterData; }
	inline TObjectPtr<UPlayerRuntimeData> GetPlayerRuntimeData() const { return PlayerRuntimeData; }

	/** ギアマネージャーを取得する */
	UWeaponManager* GetWeaponManager() const { return WeaponManager; }

	/** マップアクターマネージャーを取得する */
	UMapActorManager* GetMapActorManager() const { return MapActorManager; }
protected:

	/** セーブデータ */
	UPROPERTY()
	UMySaveGame* CurrentSaveData;

	/** ステージデータの参照 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UStageDataAsset> StageDataAsset;

	/** ステージマネージャー */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UStageManager> StageManagerClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UStageManager> StageManager;

	//ギアマネージャー
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UWeaponManager> WeaponManagerClass;

	UPROPERTY(BlueprintReadOnly)
	UWeaponManager* WeaponManager;

	/** マップアクターマネージャー */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UMapActorManager> MapActorManagerClass;

	UPROPERTY(BlueprintReadOnly)
	UMapActorManager* MapActorManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerParameterData> PlayerParameterData;

	/** Runtimeデータ 実行時に変更されるデータを計算、適用するクラス */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;
};
