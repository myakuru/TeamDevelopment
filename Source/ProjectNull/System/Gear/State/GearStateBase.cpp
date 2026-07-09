
#include "GearStateBase.h"

#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>

UGearStateBase::UGearStateBase():
	Player(nullptr),
	GearComponent(nullptr),
	Owner(nullptr)
{
	
}

void UGearStateBase::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	Player			= InPlayer;
	GearComponent	= InGearComponent;
	Owner			= InOwner;
}


