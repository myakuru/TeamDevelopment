// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectInstance.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEffectInstance 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName EffectId;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Level = 0;

};
