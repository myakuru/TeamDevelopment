#include "WaveWidget.h"
#include "Components/TextBlock.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>

void UWaveWidget::NativeConstruct()
{
	auto* CachedSubsystem = GetWorld()->GetSubsystem<UGameProgressSubsystem>();
	if (CachedSubsystem)
	{
		CachedSubsystem->OnKillCountChanged.AddUObject(this, &UWaveWidget::ShowUI);
		ShowUI(CachedSubsystem->GetKillCount(), CachedSubsystem->GetPhaseThresholds());
	}
}

void UWaveWidget::SetKillCount(int32 Value)
{
	NowKillCount = Value;
}

void UWaveWidget::SetTotalKillCount(int32 Value)
{
	NowWaveTotalKillCount = Value;
}

void UWaveWidget::ShowUI(int32 Count, int32 TotalCount)
{
	if (!KillCountText || !TotalKillCountText)
	{
		return;
	}

	NowKillCount = Count;
	NowWaveTotalKillCount = TotalCount;

	FText Text = FText::AsNumber(NowKillCount);
	KillCountText->SetText(Text);

	Text = FText::AsNumber(NowWaveTotalKillCount);
	TotalKillCountText->SetText(Text);
}
