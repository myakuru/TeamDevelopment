// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ExplosionGearStateBase.h"
#include "ExplosionGearState_Lv2.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UExplosionGearState_Lv2 : public UExplosionGearStateBase
{
	GENERATED_BODY()

public:
	UExplosionGearState_Lv2();

	void Execute(int32 CurrentGearLevel)override;
	void Update(float DeltaTime)		override;

private:

	UPROPERTY(EditAnywhere)
	TArray<FCircleSpawnData> CircleSpawnDatas;
	
};
