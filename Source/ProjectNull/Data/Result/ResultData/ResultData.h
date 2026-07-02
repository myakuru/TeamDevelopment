// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResultData.generated.h"

/**
 * 
 */

UENUM()
enum class EResultFlag : uint8
{
	ReachedMidBoss,
	ReachedFinalBoss,
};

USTRUCT()
struct FResultData {
	GENERATED_BODY()

	TArray<FName> RewardWeaponIDs;

	int32 ClearPhase = 10;
	float ClearTime = 600.0f;
	TSet<EResultFlag> ResultFlags;

};
