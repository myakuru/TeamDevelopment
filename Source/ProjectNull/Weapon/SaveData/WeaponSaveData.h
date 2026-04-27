// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../Instance/WeaponInstance.h"
#include "WeaponSaveData.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType)
struct FWeaponSaveData
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FWeaponInstance> Weapons;

	UPROPERTY()
	int64 NextUniqueID;

	UPROPERTY()
	TArray<int64> EquippedWeaponIDs;

};
