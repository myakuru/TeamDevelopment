// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../ResultData/ResultData.h"
#include "RankConditionData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNULL_API URankConditionData : public UDataAsset
{
	GENERATED_BODY()

public:


	virtual bool IsConditionMet(const FResultData& ClearData) const { return false; };

	UFUNCTION(BlueprintCallable)
	virtual FText GetConditionText() const { return ConditionText; };

protected:
	
	UPROPERTY(EditAnywhere)
	FText ConditionText;
	
};
