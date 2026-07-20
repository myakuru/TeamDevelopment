// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ExplosionGearStateBase.h"
#include "ExplosionGearState_Lv4.generated.h"

class UPlayerCutsceneComponent;

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UExplosionGearState_Lv4 : public UExplosionGearStateBase
{
	GENERATED_BODY()

public:
	UExplosionGearState_Lv4();

	void Execute(int32 CurrentGearLevel)		override;

	inline const int32 GetGearLevelIndex() const override { return kLv4Index; }


private:
	
	UPROPERTY()
	TObjectPtr<UPlayerCutsceneComponent> CutsceneComponent;
	
};
