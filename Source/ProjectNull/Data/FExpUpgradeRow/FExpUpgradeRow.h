// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "FExpUpgradeRow.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTNULL_API FFExpUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	/**
	 * @brief 強化画面のテキスト
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString UpgradeText = "";

	/**
	 * @brief 照合用のテキストID
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TextId = 0;
	
};
