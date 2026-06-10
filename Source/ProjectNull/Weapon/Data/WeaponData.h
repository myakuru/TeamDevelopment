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

	/** ギアのID (Dash) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FName WeaponID;

	/** ギアの表示名 (ダッシュギア) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FText DisplayName;

	/** ギアから取得出来る素材ID (重量フレーム) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DecomposeMaterialID;

	/** このギア作成に必要な素材 (重量フレーム等) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FMaterialRequirement> RequiredMaterialIDs;

	/** アイコン */
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon = nullptr;

	/** ギア (BP_Gear) */
    UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf <UGearBase> Gear;

};
