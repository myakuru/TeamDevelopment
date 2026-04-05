
#include "DashGearState_Lv2.h"

#include "ProjectNull/System/Gear/GearBase.h"

UDashGearState_Lv2::UDashGearState_Lv2():
	ElapsedTime(0.0f)
{
}

void UDashGearState_Lv2::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UGearStateBase::Initialize(Player, GearComponent, Gear);
	ElapsedTime = 0.0f;
}

void UDashGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	UGearStateBase::Execute(CurrentGearLevel);
}

void UDashGearState_Lv2::Update(float DeltaTime)
{
	if (!OwnerGear) { return; }

	ElapsedTime += DeltaTime;

	bool bBlocksMovement = false;

	static bool bPrevShouldDash = false;
	const bool bShouldDash = FirstDashThresholdRange.IsWithinRange(ElapsedTime) ||
		SecondDashThresholdRange.IsWithinRange(ElapsedTime);


	if (bShouldDash)
	{
		Dash();
		bBlocksMovement = true;
	}

	if (bShouldDash != bPrevShouldDash) 
	{
		if (bShouldDash) 
		{
			PlayDashEffect();
		}
	}

	bPrevShouldDash = bShouldDash;

	OwnerGear->SetBlocksMovement(bBlocksMovement);

}
