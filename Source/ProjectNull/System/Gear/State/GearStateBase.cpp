
#include "GearStateBase.h"

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