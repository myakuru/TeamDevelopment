// Fill out your copyright notice in the Description page of Project Settings.


#include "FGameTimer.h"
#include "TimerManager.h"
#include "Engine/World.h"

FGameTimer::FGameTimer()
{
}

FGameTimer::~FGameTimer()
{
	// 破棄時に必ずタイマーを止める（CreateRaw は寿命管理されないため）
	StopTimer();
}

void FGameTimer::StartTimer(UWorld* InWorld, float StartTime)
{
	// 既に動いているタイマーがあれば止めてから開始する（多重登録防止）
	StopTimer();

	World = InWorld;
	RemainingTime = StartTime;

	if (!World.IsValid()) return;

	// タイマーを開始（非UObjectなので CreateRaw でバインド）
	World->GetTimerManager().SetTimer(
		CountdownTimerHandle,
		FTimerDelegate::CreateRaw(this, &FGameTimer::CountDownTimer),
		1.0f,
		true);
}

void FGameTimer::StopTimer()
{
	if (World.IsValid())
	{
		World->GetTimerManager().ClearTimer(CountdownTimerHandle);
	}
}

void FGameTimer::CountDownTimer()
{
	if (RemainingTime <= 0.0f)
	{
		// タイマーが0になった場合
		StopTimer();
		OnFinished.Broadcast();
		return;
	}

	// 残り時間を通知して1秒減らす
	OnTick.Broadcast(RemainingTime);
	RemainingTime -= 1.0f;
}
