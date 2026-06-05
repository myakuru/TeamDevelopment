#include "HealGear.h"

UHealGear::UHealGear()
{
}

void UHealGear::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(Player, GearComponent);
}

void UHealGear::Execute(int32 CurrentGearLevel)
{
	UGearBase::Execute(CurrentGearLevel);
}

void UHealGear::Update(float DeltaTime)
{
	UGearBase::Update(DeltaTime);
}
