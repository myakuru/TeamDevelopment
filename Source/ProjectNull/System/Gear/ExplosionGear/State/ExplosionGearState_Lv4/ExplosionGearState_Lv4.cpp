// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearState_Lv4.h"
#include <ProjectNull/Component/PlayerCutsceneComponent/PlayerCutsceneComponent.h>
#include <ProjectNull/Core/NullGameplayTags.h>

UExplosionGearState_Lv4::UExplosionGearState_Lv4()
{
	CutsceneComponent = CreateDefaultSubobject<UPlayerCutsceneComponent>("Cutscene");
}

void UExplosionGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!CutsceneComponent) { return; }
	CutsceneComponent->PlayCutScene(NullGameplayTags::Cutscene_ExplosionGearLev4);
}
