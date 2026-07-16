// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/** 毎秒の残り時間通知（引数：残り秒数） */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTimerTick, float /*RemainingTime*/);

/** カウントダウン終了通知 */
DECLARE_MULTICAST_DELEGATE(FOnTimerFinished);

/**
 * カウントダウンのロジックだけを持つタイマー。
 * UI や StageManager を知らず、通知はデリゲートで外に渡す。
 */
class PROJECTNULL_API FGameTimer
{
public:
	FGameTimer();
	~FGameTimer();

	/** カウントダウンを開始する（World は TimerManager の取得に必要） */
	void StartTimer(UWorld* InWorld, float StartTime);

	/** カウントダウンを停止する */
	void StopTimer();

	/** 毎秒呼ばれる（残り時間を渡す） */
	FOnTimerTick OnTick;

	/** 残り時間が0になったら呼ばれる */
	FOnTimerFinished OnFinished;

private:
	/** TimerManager から毎秒呼ばれる本体 */
	void CountDownTimer();

	/** TimerManager を取得するための World */
	TWeakObjectPtr<UWorld> World;

	/** 残り時間 */
	float RemainingTime = 0.0f;

	/** ワールドから取得したタイマーハンドル */
	FTimerHandle CountdownTimerHandle;
};
