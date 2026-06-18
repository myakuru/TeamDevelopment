// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResultData.generated.h"

/**
 * 
 */
USTRUCT()
struct FResultData {
	GENERATED_BODY()

	TArray<FName> RewardWeaponIDs;

	int32 ClearPhase = 7;
	bool bReachedMidBossPhase = false;
	bool bReachedFinalPhase = false;
	float ClearTime = 600.0f;
};
