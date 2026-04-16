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

protected:

	UFUNCTION(BlueprintCallable, Category = "Game Timer")
	void StartTimer(float Duration);

	UPROPERTY()
	FString TimerText;

private:

	void CountDownTimer();

	float RemainingTime;

	// タイマーハンドル
	FTimerHandle CountdownTimerHandle;
	
};
