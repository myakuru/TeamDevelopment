// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RankConditionData.h"
#include "MidBossKillConditionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UMidBossKillConditionData : public URankConditionData
{
	GENERATED_BODY()

public:

	bool IsConditionMet(const FResultData& ClearData) const override {
		return ClearData.ResultFlags.Contains(EResultFlag::ReachedMidBoss);
	}
	
};
