// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "ClearRankData.generated.h"

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
	int32 ClearPhase = 0;

	UPROPERTY(EditAnywhere)
	bool bReachedMidBossPhase = false;

	UPROPERTY(EditAnywhere)
	bool bReachedFinalPhase = false;

	UPROPERTY(EditAnywhere)
	float ClearTime = 0.0f;

};