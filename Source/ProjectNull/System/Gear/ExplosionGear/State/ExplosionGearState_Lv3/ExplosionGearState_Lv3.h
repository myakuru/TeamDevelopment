// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ExplosionGearStateBase.h"
#include "ExplosionGearState_Lv3.generated.h"

/**
 * 
 */



UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UExplosionGearState_Lv3 : public UExplosionGearStateBase
{
	GENERATED_BODY()

public:
	UExplosionGearState_Lv3();

	void Execute(int32 CurrentGearLevel)override;
	void Update(float DeltaTime)		override;

private:
	UPROPERTY(EditAnywhere)
	FExplosionSpawnData FirstExplosionData;

	UPROPERTY(EditAnywhere)
	double JumpPow = 0.0;

	UPROPERTY(EditAnywhere)
	TArray<FCircleSpawnData> CircleSpawnDatas;

	FTimerHandle FirstExplosionTimerHandle;

};
