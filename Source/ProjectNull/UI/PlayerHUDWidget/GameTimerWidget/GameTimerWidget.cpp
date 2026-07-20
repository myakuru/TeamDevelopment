#include "GameTimerWidget.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Stage/Manager/StageManager.h>

void UGameTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// StageManagerが所有するタイマーの通知を購読する（表示のみ）
	if (USuperGameInstance* GameInstance = GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		if (UStageManager* StageManager = GameInstance->GetStageManagerSubsystem())
		{
			StageManager->GetOnStageTimerTick().AddUObject(this, &UGameTimerWidget::HandleTimerTick);
			StageManager->GetOnStageTimerFinished().AddUObject(this, &UGameTimerWidget::HandleTimerFinished);
		}
	}
}

void UGameTimerWidget::NativeDestruct()
{
	// 購読を解除する
	if (UWorld* World = GetWorld())
	{
		if (USuperGameInstance* GameInstance = World->GetGameInstance<USuperGameInstance>())
		{
			if (UStageManager* StageManager = GameInstance->GetStageManagerSubsystem())
			{
				StageManager->GetOnStageTimerTick().RemoveAll(this);
				StageManager->GetOnStageTimerFinished().RemoveAll(this);
			}
		}
	}

	Super::NativeDestruct();
}

void UGameTimerWidget::HandleTimerTick(float RemainingTime)
{
	// タイマーの残り時間を更新
	int32 Hours = FMath::FloorToInt(RemainingTime / 3600);
	int32 Minutes = FMath::FloorToInt(RemainingTime / 60);
	int32 Seconds = FMath::FloorToInt(RemainingTime) % 60;
	TimerText = FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
}

void UGameTimerWidget::HandleTimerFinished()
{
	TimerText = TEXT("Time's Up!");
}
