
#include "DashGearState_Lv2.h"

#include <ProjectNull/System/Gear/GearBase.h>

UDashGearState_Lv2::UDashGearState_Lv2():
	FirstDashDuration(0.f),
	FirstDashToDelay(0.f),
	SecondDashDuration(0.f),
	bShouldDash(false),
	FirstDashTimerHandle(FTimerHandle()),
	FirstDashToDelayTimerHandle(FTimerHandle())
{
}

void UDashGearState_Lv2::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	UDashGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);

	InitializeGearDuration();
	
}

void UDashGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	UDashGearStateBase::Execute(CurrentGearLevel);

	bShouldDash = true;

	GetWorld()->GetTimerManager().SetTimer(
		FirstDashTimerHandle,
		this,
		&UDashGearState_Lv2::EndFirstDash,
		FirstDashDuration,
		false);
}

void UDashGearState_Lv2::Update(float DeltaTime)
{
	UGearStateBase::Update(DeltaTime);
	
	if (bShouldDash)
	{
		Dash();
	}

	const bool BlocksMovement = bShouldDash;

	Owner->SetBlocksMovement(BlocksMovement);
}

void UDashGearState_Lv2::End()
{
	UDashGearStateBase::End();
	bShouldDash = false;
}

void UDashGearState_Lv2::InitializeGearDuration()
{
	if (!Owner) { return; }
	float TotalDutation = 0.f;

	TotalDutation += FirstDashDuration + FirstDashToDelay + SecondDashDuration;
	Owner->SetGearDuration(TotalDutation, kLv2Index);
}

void UDashGearState_Lv2::EndFirstDash()
{
	bShouldDash = false;

	EndDash();

	GetWorld()->GetTimerManager().SetTimer(
		FirstDashToDelayTimerHandle,
		this,
		&UDashGearState_Lv2::StartSecondDash,
		FirstDashToDelay,
		false);
}

void UDashGearState_Lv2::StartSecondDash()
{
	bShouldDash = true;
	ExecuteDash();
}
