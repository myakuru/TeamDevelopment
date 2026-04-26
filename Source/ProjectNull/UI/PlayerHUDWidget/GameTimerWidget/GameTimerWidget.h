// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimerWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	/** タイマーを開始する関数 */
	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void StartTimer(float Duration);

protected:

	/** タイマーのテキストを表示するための画像 */
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FString TimerText;

private:

	void CountDownTimer();

	/** 残り時間 */
	float RemainingTime = 0.0f;

	/** ワールドから取得したタイマー */
	FTimerHandle CountdownTimerHandle;
	
};
