// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultManager.h"
#include <ProjectNull/Data/Result/RewardData/RewardData.h>
#include <ProjectNull/Weapon/Data/EffectPoolDataAsset/EffectPoolDataAsset.h>
#include <ProjectNull/Data/Result/RankConditionData/RankConditionData.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>

void UResultManager::Initialize()
{
	if (!ClearRankDataTable)return;
	SortedClearRankDatas.Empty();

	// ClearRankDataTableを評価順に並べなおして配列として保持させる
	{
		// 全行取得
		TArray<FClearRankData*> rows;
		ClearRankDataTable->GetAllRows(TEXT(""), rows);

		// 配列に格納
		for (const FClearRankData* row : rows) {
			SortedClearRankDatas.Add(*row);
		}

		// RankOrder順になるようにソート
		SortedClearRankDatas.Sort(
			[](const FClearRankData& A, const FClearRankData& B)
			{
				return A.RankOrder < B.RankOrder;
			}
		);
	}
}

void UResultManager::SetResultData(FResultData ResultData)
{
	CurrentResultData = ResultData;
	RewardWeapons.Empty();
	EvoluteReward();
}

void UResultManager::EvoluteReward()
{
	FDataTableRowHandle rewardRow = SortedClearRankDatas[0].RewardRow;

	// 評価ランク決定
	for (int i = 0; i < SortedClearRankDatas.Num(); i++) {

		if (!SortedClearRankDatas[i].ConditionData->IsConditionMet(CurrentResultData))break;

		rewardRow = SortedClearRankDatas[i].RewardRow;
	}

	// 決定した評価ランクの行を取得
	FRewardData* data =
		rewardRow.GetRow<FRewardData>(
			TEXT(__FUNCTION__)
		);

	// 決定した評価ランクに応じて取得した武器に追加効果を付与する
	for (FName weaponID : CurrentResultData.RewardWeaponIDs) {
		GenerateWeapon(weaponID, data);
	}

	USuperGameInstance* gameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!gameInstance)return;

	UWeaponManager* weaponManager = gameInstance->GetWeaponManager();
	if (!weaponManager)return;
	for (FWeaponInstance weapon : RewardWeapons) {
		weaponManager->AddWeapon(weapon);
	}
}

void UResultManager::GenerateWeapon(FName WeaponID, FRewardData* RewardData)
{
	// 付与する追加効果の数を抽選
	int32 effectCount = 0;
	{
		int32 totalWeight = 0;
		for (const FWeightedEffectCount& weightedEffectCount : RewardData->WeightedEffectCounts) {
			totalWeight += weightedEffectCount.Weight;
		}

		const int32 randomValue = FMath::RandRange(0, totalWeight - 1);

		int32 currentWeight = 0;
		for (const FWeightedEffectCount& weightedEffectCount : RewardData->WeightedEffectCounts) {
			currentWeight += weightedEffectCount.Weight;

			if (randomValue < currentWeight) {
				effectCount = weightedEffectCount.EffectCount;
				break;
			}
		}
	}

	TArray<FEffectInstance> effects;

	//付与する追加効果の抽選リスト
	TArray<TObjectPtr<UEffectData>> effectList = EffectPool->GetEffects();
	
	// 追加効果の種類を抽選し、レベルも抽選する
	for (int i = 0; i < effectCount; i++) {
		FEffectInstance effect;

		// 追加効果の種類を抽選し、以降追加効果が重複しないように抽選リストから付与した追加効果を外す
		int32 randIndex = FMath::RandRange(0, effectList.Num() - 1);
		effect.Data = effectList[randIndex];
		effectList.RemoveAt(randIndex);

		int32 totalWeight = 0;
		for (const FWeightedEffectLevel& weightedEffectLevel : RewardData->WeightedEffectLevels) {
			totalWeight += weightedEffectLevel.Weight;
		}

		const int32 randomValue = FMath::RandRange(0, totalWeight - 1);

		int32 currentWeight = 0;
		for (const FWeightedEffectLevel& weightedEffectLevel : RewardData->WeightedEffectLevels) {
			currentWeight += weightedEffectLevel.Weight;

			if (randomValue < currentWeight) {
				effect.Level = weightedEffectLevel.EffectLevel;
				break;
			}
		}

		effects.Add(effect);
	}

	FWeaponInstance rewardWeapon;
	rewardWeapon.WeaponId = WeaponID;
	rewardWeapon.Effects = effects;

	RewardWeapons.Add(rewardWeapon);
}
