// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include"../Instance/WeaponInstance.h"
#include"../Instance/WeaponMaterialInstance.h"
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
	int64 NextWeaponUniqueID = 0;

	UPROPERTY()
	TArray<int64> EquippedWeaponIDs;

	UPROPERTY()
	TArray<FWeaponMaterialInstance> Materials;

	UPROPERTY()
	int64 NextMaterialUniqueID = 0;

};
