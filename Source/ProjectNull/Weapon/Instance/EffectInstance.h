// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EffectInstance.generated.h"

class UEffectData;
class UUEffectBase;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FEffectInstance 
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
	UEffectData* Data;

	UPROPERTY()
	UUEffectBase* RuntimeEffect;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 Level = 0;

	void Initialize(UObject* a_Outer);

};
