// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../RankConditionData.h"
#include "ClearTimeConditionData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UClearTimeConditionData : public URankConditionData
{
	GENERATED_BODY()

public:

	bool IsConditionMet(const FResultData& ClearData) const override {
		return ClearData.ClearTime <= TargetTime;
	}

private:

	UPROPERTY(EditAnywhere)
	int32 TargetTime = 0;
	
};
