// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponData.generated.h"

class UGearBase;

/**
 * 
 */

USTRUCT(BlueprintType)
struct FMaterialRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName MaterialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 1;

};

USTRUCT(BlueprintType)
struct FWeaponData : public FTableRowBase
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName WeaponID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DecomposeMaterialID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialRequirement> RequiredMaterialIDs;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf <UGearBase> Gear;

};
