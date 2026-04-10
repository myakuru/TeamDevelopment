
#include "GearStateBase.h"


UGearStateBase::UGearStateBase()
{
	
}

void UGearStateBase::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	OwnerPlayer = Player;
	OwnerGearComponent = GearComponent;
	OwnerGear = Gear;
}
