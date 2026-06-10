// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../EffectData.h"
#include "EffectPoolDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UEffectPoolDataAsset : public UDataAsset
{
public:
	GENERATED_BODY()

	TArray<TObjectPtr<UEffectData>> GetEffects() { return Effects; }

private:

	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UEffectData>> Effects;
	
};
