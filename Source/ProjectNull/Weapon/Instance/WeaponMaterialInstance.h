// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EffectInstance.h"
#include "CoreMinimal.h"
#include "WeaponMaterialInstance.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FWeaponMaterialInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int64 UniqueId = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponMaterialId;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEffectInstance> Effects;

};
