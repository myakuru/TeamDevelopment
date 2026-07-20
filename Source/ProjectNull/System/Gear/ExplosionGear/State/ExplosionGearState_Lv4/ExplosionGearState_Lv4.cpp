// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearState_Lv4.h"
#include <ProjectNull/Component/PlayerCutsceneComponent/PlayerCutsceneComponent.h>
#include <ProjectNull/Core/NullGameplayTags.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

UExplosionGearState_Lv4::UExplosionGearState_Lv4()
{
}

void UExplosionGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (Player)
	{
		CutsceneComponent = Player->GetPlayerCutsceneComponent();	
	}
	
	if (!CutsceneComponent) return;
	
	CutsceneComponent->PlayCutScene(NullGameplayTags::Cutscene_ExplosionGearLev4);
}
