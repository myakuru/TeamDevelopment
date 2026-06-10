#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include <ProjectNull/SaveGame/StageProgressData.h>
#include "StageManager.generated.h"

class UStageDataAsset;
class UMySaveGame;

/**
 * ステージの状況を管理するマネージャー
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UStageManager : public UObject
{
	GENERATED_BODY()
	
public:

	void LoadFromSaveData(UMySaveGame* inSaveGame);

	void SaveToData(UMySaveGame* inSaveGame);

	UFUNCTION()
	void StageStart(int32 inNowStageIndex);

	UFUNCTION()
	void StageClear();

	UFUNCTION()
	void AddAcquiredWeapon(const FText& WeaponName) {
		AcquiredWeapons.Add(WeaponName);

		//ログ出力
		UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
		UE_LOG(LogTemp, Log, TEXT("AcquiredWeaponNum: %d"), AcquiredWeapons.Num());
		for(int i  = 0; i < AcquiredWeapons.Num(); i++)
		{
			UE_LOG(LogTemp, Log, TEXT("AcquiredWeapon%d: %s"), i, *AcquiredWeapons[i].ToString());
		}
		UE_LOG(LogTemp, Log, TEXT("----------------------------------"));
	}

	/* ゲッター */
	/** ステージデータを取得する */
	inline TObjectPtr<UStageDataAsset> GetStageDataAsset() const { return StageDataAsset; }
	
	/* 現在のステージインデックスを取得する */
	int32 GetNowStageIndex() const { return NowStageIndex; }

	/* セッター */

private:

	/** ステージデータの参照 */
	UPROPERTY(EditAnywhere)
	const TObjectPtr<UStageDataAsset> StageDataAsset;

	/** ステージの進行状況リスト */
	TArray<FStageProgressData>* StageProgressList;

	/** 現在のステージのインデックス */
	int32 NowStageIndex;

	/** プレイ中に取得した武器の一時保持用配列 */
	UPROPERTY()
	TArray<FText> AcquiredWeapons;
};
