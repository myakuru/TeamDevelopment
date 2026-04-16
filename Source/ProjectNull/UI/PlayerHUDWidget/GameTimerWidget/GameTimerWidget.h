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

	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void StartTimer(float Duration);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FString TimerText;

private:

	void CountDownTimer();

	float RemainingTime = 0.0f;	// タイマーハンドル
	FTimerHandle CountdownTimerHandle;
	
};
