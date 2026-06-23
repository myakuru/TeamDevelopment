#include "HitStopComponent.h"

UHitStopComponent::UHitStopComponent():
	OriginalDilation(1.f)
{
}

void UHitStopComponent::BeginPlay()
{
	Super::BeginPlay();

	auto* Owner = GetOwner();
	if (!Owner) { return; }
	OriginalDilation = Owner->CustomTimeDilation;
}

void UHitStopComponent::StartHitStop(float Duration, float TimeDilation)
{
	auto* Owner = GetOwner();
	if (!Owner) { return; }

	auto& TimerManager = GetWorld()->GetTimerManager();

	TimerManager.ClearTimer(HitStopTimerHandle);
	Owner->CustomTimeDilation = TimeDilation;

	TimerManager.SetTimer(
		HitStopTimerHandle,
		this,
		&UHitStopComponent::EndHitStop,
		Duration,
		false
	);
}

void UHitStopComponent::EndHitStop()
{
	if (auto* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = OriginalDilation;
	}
}