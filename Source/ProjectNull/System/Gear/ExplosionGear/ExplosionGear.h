// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GearBase.h"
#include "ExplosionGear.generated.h"

/** 爆発ギアクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UExplosionGear : public UGearBase
{
	GENERATED_BODY()
public:
	UExplosionGear();
public:

	void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent)	override;
	void Execute(int32 CurrentGearLevel)													override;
	void Update(float DeltaTime)															override;
	
};
