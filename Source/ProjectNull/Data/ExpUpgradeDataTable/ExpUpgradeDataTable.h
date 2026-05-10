// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ExpUpgradeDataTable.generated.h"

/**
 * 強化画面のテキストとIdを管理するデーターテーブル
 */
USTRUCT(BlueprintType)
struct PROJECTNULL_API FExpUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText UpgradeText;
};
