// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearState_Lv1.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull\Actor\Explosion\ExplosionGearSkill.h>
#include "Kismet\GameplayStatics.h"

UExplosionGearState_Lv1::UExplosionGearState_Lv1()
{
}

void UExplosionGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	Super::Execute(CurrentGearLevel);

	SpawnExplosionsInCircle(CircularExplosionData, ExplosionSpawnRadius, ExplosionCount);

}

void UExplosionGearState_Lv1::Update(float DeltaTime)
{
}
