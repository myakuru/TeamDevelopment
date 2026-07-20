// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameTimerWidget.generated.h"

class UImage;

/**
 * ステージタイマーの残り時間を表示するだけのWidget。
 * カウントダウン自体は UStageManager が所有する FGameTimer が行う。
 */
UCLASS()
class PROJECTNULL_API UGameTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	/** タイマーのテキストを表示するための画像 */
	UPROPERTY(BlueprintReadOnly, Category = "Timer")
	FString TimerText;

private:

	/** 残り時間を受け取って表示テキストを更新する */
	void HandleTimerTick(float RemainingTime);

	/** タイマー終了時にテキストを更新する */
	void HandleTimerFinished();

};
