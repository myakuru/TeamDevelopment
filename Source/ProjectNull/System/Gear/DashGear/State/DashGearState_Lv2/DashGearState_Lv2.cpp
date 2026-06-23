
#include "DashGearState_Lv2.h"

#include <ProjectNull/System/Gear/GearBase.h>

UDashGearState_Lv2::UDashGearState_Lv2():
	ElapsedTime(0.f)
{
}

void UDashGearState_Lv2::Initialize(APlayerBase* InPlayer, UPlayerGearComponent* InGearComponent, UGearBase* InOwner)
{
	UDashGearStateBase::Initialize(InPlayer, InGearComponent, InOwner);
}

void UDashGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	UDashGearStateBase::Execute(CurrentGearLevel);

	/*GetWorld()->GetTimerManager().SetTimer(
		FirstDashTimerHandle,
		);*/


	ElapsedTime = 0.0f;
}

void UDashGearState_Lv2::Update(float DeltaTime)
{
	if (!Owner) { return; }

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
			//PlayDashEffect();
		}
	}

	bPrevShouldDash = bShouldDash;

	Owner->SetBlocksMovement(bBlocksMovement);

}
