// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ExpUpgradeDataTable.generated.h"

/** 1レベル分の強化情報 */
USTRUCT(BlueprintType)
struct FExpUpgradeLevelData
{
	GENERATED_BODY()

	/** 説明文 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	/** 攻撃倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackMultiplier = 1.0f;
};


/**
 * 強化画面のテキストとIdを管理するデーターテーブル
 */
USTRUCT(BlueprintType)
struct PROJECTNULL_API FExpUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 攻撃倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FExpUpgradeLevelData> UpgradeLevels;
};
