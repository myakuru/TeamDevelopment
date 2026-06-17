// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../../Data/Result/ClearRankData/ClearRankData.h"
#include "../../../Data/Result/ResultData/ResultData.h"
#include "../../../Data/Result/RewardData/RewardData.h"
#include "../../../Weapon/Instance/WeaponInstance.h"

#include "ResultManager.generated.h"

class UEffectPoolDataAsset;

/** クリア時のスコアに応じて報酬を算出するクラス */
UCLASS()
class PROJECTNULL_API UResultManager : public UObject
{
	GENERATED_BODY()

public:

	void Initialize();
	void SetResultData(const FResultData& ResultData);
	TArray<FWeaponInstance> GetRewardWeapons() { return RewardWeapons; }

	FResultData GetResultData() { return CurrentResultData; }

	TArray<FClearRankData> GetSortedClearRankDatas() { return SortedClearRankDatas; }

private:

	void EvoluteReward();
	void GenerateWeapon(FName WeaponID,FRewardData* RewardData);

	/** クリアランクデータテーブルをランク順にソートした配列 */
	TArray<FClearRankData> SortedClearRankDatas;

	/** 受け取ったクリアデータ */
	FResultData CurrentResultData;

	/** 追加効果を算出した報酬ギアリスト */
	TArray<FWeaponInstance> RewardWeapons;

	/** クリアランクごとの達成条件や報酬をまとめたデータテーブル */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UDataTable> ClearRankDataTable;

	/** 追加効果すべての種類を保持しているデータアセット */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEffectPoolDataAsset> EffectPool;
	
};
