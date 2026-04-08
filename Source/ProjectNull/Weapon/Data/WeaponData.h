// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponData.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UWeaponData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName WeaponId;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	FText Name;
	
};
