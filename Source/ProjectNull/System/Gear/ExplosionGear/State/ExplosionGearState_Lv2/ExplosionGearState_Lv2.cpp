// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearState_Lv2.h"

UExplosionGearState_Lv2::UExplosionGearState_Lv2()
{
}

void UExplosionGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	Super::Execute(CurrentGearLevel);

	SpawnExplosions();

	for (const FCircleSpawnData& data : CircleSpawnDatas) {
		SpawnExplosionsInCircle(data.ExplosionData, data.Radius, data.Count);
	}
}

void UExplosionGearState_Lv2::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}
