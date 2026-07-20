// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RankConditionData.h"
#include "PhaseReachConditionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UPhaseReachConditionData : public URankConditionData
{
	GENERATED_BODY()

protected:

	bool IsConditionMet(const FResultData& ClearData) const override {
		return ClearData.ClearPhase >= TargetPhase;
	}

private:

	UPROPERTY(EditAnywhere)
	int32 TargetPhase = 0;
	
};
