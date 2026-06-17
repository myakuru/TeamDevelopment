// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RankConditionData.h"
#include "FinalPhaseReachConditionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UFinalPhaseReachConditionData : public URankConditionData
{
	GENERATED_BODY()
	
public:

	bool IsConditionMet(const FResultData& ClearData) const override {
		return ClearData.bReachedFinalPhase;
	}

};
