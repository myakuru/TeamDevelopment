// Fill out your copyright notice in the Description page of Project Settings.
#include "GameTimerWidget.h"
#include "TimerManager.h"

void UGameTimerWidget::StartTimer(float Duration)
{
	RemainingTime = Duration;

	// タイマーを開始
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UGameTimerWidget::CountDownTimer, 1.0f, true);

}

void UGameTimerWidget::CountDownTimer()
{
	if (RemainingTime <= 0)
	{
		// タイマーが0になった場合の処理
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		TimerText = TEXT("Time's Up!");
	}
	else
	{
		// タイマーの残り時間を更新
		int32 Hours = FMath::FloorToInt(RemainingTime / 3600);
		int32 Minutes = FMath::FloorToInt(RemainingTime / 60);
		int32 Seconds = FMath::FloorToInt(RemainingTime) % 60;
		TimerText = FString::Printf(TEXT("%02d:%02d:%02d"), Hours, Minutes, Seconds);
		RemainingTime -= 1.0f; // 1秒減らす
	}
}

