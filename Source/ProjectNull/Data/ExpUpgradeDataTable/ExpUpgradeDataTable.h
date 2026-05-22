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

	/** 各レベルの表示テキスト（Index == レベル） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FText> UpgradeTexts;

	/** ユーザー向け表示名 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText DisplayName;
};
