// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Data/Result/ResultData/ResultData.h"
#include "EvaluationPanel.generated.h"

class UTextBlock;
class UVerticalBox;
class UResultAchievementWidget;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UEvaluationPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct()override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReachedPhase;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> AchieveList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UResultAchievementWidget> ResultAchievementWidgetClass;

	TQueue<TObjectPtr<UResultAchievementWidget>> Achievements;

	FResultData ResultData;

	bool bPlayingClearTimeAnimation = false;
	bool bPlayingReachedPhaseAnimation = false;
	bool bPlayingAchievemtsCheckAnimation = false;

	float CurrentDisplayTime = 0.0f;
	float TargetTime = 0.0f;

	int32 CurrentDisplayPhase = 0;
	int32 TargetPhase = 0;

	UPROPERTY(EditAnywhere)
	float AnimationDuration = 1.0f;
	float ElapsedTime = 0.0f;

	UPROPERTY(EditAnywhere)
	float AchievementClearCheckDuration = 1.0f;

	void ClearTimeAnimation(float InDeltaTime);
	void ReachedPhaseAnimation(float InDeltaTime);
	void AchievementAnimation(float InDeltaTime);

	
};
