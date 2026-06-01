// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGear.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

UExplosionGear::UExplosionGear()
{
}

void UExplosionGear::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(Player, GearComponent);
}

void UExplosionGear::Execute(int32 CurrentGearLevel)
{
	UGearBase::Execute(CurrentGearLevel);
}

void UExplosionGear::Update(float DeltaTime)
{
	UGearBase::Update(DeltaTime);
}
