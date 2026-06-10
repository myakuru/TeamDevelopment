// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "RewardData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeightedEffectCount {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 EffectCount = 0;

	UPROPERTY(EditAnywhere)
	int32 Weight = 1;
};

USTRUCT(BlueprintType)
struct FWeightedEffectLevel {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 EffectLevel = 0;

	UPROPERTY(EditAnywhere)
	int32 Weight = 1;

};

USTRUCT(Blueprintable)
struct FRewardData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FWeightedEffectCount> WeightedEffectCounts;

	UPROPERTY(EditAnywhere)
	TArray<FWeightedEffectLevel> WeightedEffectLevels;
};