// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EffectData.generated.h"

class UUEffectBase;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNULL_API UEffectData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUEffectBase> EffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
};
