// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ClearRankData.generated.h"

class URankConditionData;

/**
 * 
 */
USTRUCT(Blueprintable)
struct FClearRankData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	int32 RankOrder = 0;

	UPROPERTY(EditAnywhere)
	FDataTableRowHandle RewardRow;

	UPROPERTY(EditAnywhere)
	TObjectPtr<URankConditionData> ConditionData;

};