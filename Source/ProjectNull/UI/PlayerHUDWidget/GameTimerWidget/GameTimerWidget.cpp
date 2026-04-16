// Fill out your copyright notice in the Description page of Project Settings.
#include "GameTimerWidget.h"
#include "TimerManager.h"

void UGameTimerWidget::StartTimer(float Duration)
{
	RemainingTime = Duration;

	// ワールドからタイマーを取得して、CountDownTimer関数を一定間隔で呼び出す
	GetWorld()->GetTimerManager().SetTimer(CountdownTimerHandle, this, &UGameTimerWidget::CountDownTimer, 1.0f, true);

}

void UGameTimerWidget::CountDownTimer()
{
	if (RemainingTime <= 0)
	{
		// タイマーが終了した場合の処理
		GetWorld()->GetTimerManager().ClearTimer(CountdownTimerHandle);
		TimerText = TEXT("Time's Up!");
	}
	else
	{
		// タイマーがまだ残っている場合の処理
		int32 Minutes = FMath::FloorToInt(RemainingTime / 60);
		int32 Seconds = FMath::FloorToInt(RemainingTime) % 60;
		TimerText = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		RemainingTime -= 1.0f; // 1秒減らす
	}
}

