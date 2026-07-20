// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "MenuData.generated.h"

/**
 * 
 */

USTRUCT(Blueprintable,BlueprintType)
struct FMenuData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Name;

	UPROPERTY(EditAnywhere,BlueprintReadOnly, meta = (MultiLine = true))
	FText Description;

};
