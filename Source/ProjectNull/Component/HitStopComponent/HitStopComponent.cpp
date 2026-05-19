#include "HitStopComponent.h"

void UHitStopComponent::StartHitStop(float Duration, float TimeDilation)
{
	AActor* Owner = GetOwner();
	if (!Owner) { return; }

	GetWorld()->GetTimerManager().ClearTimer(HitStopTimerHandle);

	OriginalDilation = Owner->CustomTimeDilation;
	Owner->CustomTimeDilation = TimeDilation;

	GetWorld()->GetTimerManager().SetTimer(
		HitStopTimerHandle,
		this,
		&UHitStopComponent::EndHitStop,
		Duration,
		false
	);
}

void UHitStopComponent::EndHitStop()
{
	if (AActor* Owner = GetOwner())
	{
		Owner->CustomTimeDilation = OriginalDilation;
	}
}