// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include"EffectInstance.h"
#include "CoreMinimal.h"
#include "WeaponInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponInstance 
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 UniqueId = 0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName WeaponId;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FEffectInstance> Effects;

};