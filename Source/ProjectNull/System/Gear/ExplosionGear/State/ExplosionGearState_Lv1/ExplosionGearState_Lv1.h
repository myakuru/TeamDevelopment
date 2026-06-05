// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../ExplosionGearStateBase.h"
#include "ExplosionGearState_Lv1.generated.h"

/**
 * 
 */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UExplosionGearState_Lv1 : public UExplosionGearStateBase
{
	GENERATED_BODY()

public:
	UExplosionGearState_Lv1();

	void Execute(int32 CurrentGearLevel)override;
	void Update(float DeltaTime)		override;

private:

	/** 円周上に設置する爆発データ */
	UPROPERTY(EditAnywhere)
	FExplosionSpawnData CircularExplosionData;

	/** 爆発を設置する円の半径 */
	UPROPERTY(EditAnywhere)
	float ExplosionSpawnRadius = 0.0f;
	
	/** 円周上に設置する爆発の数 */
	UPROPERTY(EditAnywhere)
	int32 ExplosionCount = 0;
	
};
