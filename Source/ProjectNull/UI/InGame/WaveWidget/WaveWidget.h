#pragma once
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WaveWidget.generated.h"

class UTextBlock;

/**
 *
 */
UCLASS()
class PROJECTNULL_API UWaveWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeConstruct()override;

	/** 敵を倒した数セット*/
	void SetKillCount(int32 Value = 1);
	void SetTotalKillCount(int32 Value = 1);

	void ShowUI(int32 Count, int32 TotalCount);

	// WidgetBlueprint側のTextBlock名をKillCountTexにする
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> KillCountText = nullptr;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TotalKillCountText = nullptr;

private:

	int32 NowKillCount = 0;
	int32 NowWaveTotalKillCount = 0;

};
