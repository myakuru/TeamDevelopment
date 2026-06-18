// Fill out your copyright notice in the Description page of Project Settings.


#include "EvaluationPanel.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Result/ResultManager/ResultManager.h>
#include <ProjectNull/UI/OutGame/Result/EvaluationPanel/ResultAcievementWidget/ResultAchievementWidget.h>
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"

void UEvaluationPanel::NativeConstruct()
{
	Super::NativeConstruct();

	USuperGameInstance* gameInstance = GetGameInstance<USuperGameInstance>();
	if (!gameInstance)return;

	UResultManager* resultManager = gameInstance->GetResultManager();
	if (!resultManager)return;

	ResultData = resultManager->GetResultData();

	TargetTime = ResultData.ClearTime;
	TargetPhase = ResultData.ClearPhase;

	if (!ResultAchievementWidgetClass || !AchieveList) return;

	for (const FClearRankData& clearRankData : resultManager->GetSortedClearRankDatas()) {
		UResultAchievementWidget* newItem = CreateWidget<UResultAchievementWidget>(GetWorld(), ResultAchievementWidgetClass);
		newItem->Initialize(clearRankData.ConditionData);

		Achievements.Enqueue(newItem);
		AchieveList->AddChild(newItem);
	}

	bPlayingClearTimeAnimation = true;
	bPlayingReachedPhaseAnimation = true;
	bPlayingAchievemtsCheckAnimation = true;
}

void UEvaluationPanel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (bPlayingClearTimeAnimation) {
		ClearTimeAnimation(InDeltaTime);
		return;
	}

	if (bPlayingReachedPhaseAnimation) {
		ReachedPhaseAnimation(InDeltaTime);
		return;
	}

	if (bPlayingAchievemtsCheckAnimation) {
		AchievementAnimation(InDeltaTime);
		return;
	}
}

void UEvaluationPanel::ClearTimeAnimation(float InDeltaTime)
{
	ElapsedTime += InDeltaTime;
	float Alpha = FMath::Clamp(ElapsedTime / AnimationDuration, 0.0f, 1.0f);

	CurrentDisplayTime = FMath::Lerp(0.0f, TargetTime, Alpha);

	int32 Minutes = static_cast<int32>(CurrentDisplayTime) / 60;
	int32 Seconds = static_cast<int32>(CurrentDisplayTime) % 60;
	int32 Centiseconds = static_cast<int32>(CurrentDisplayTime * 100.0f) % 100;

	FString Text =
		FString::Printf(TEXT("%02d:%02d:%02d"), Minutes, Seconds, Centiseconds);

	ClearTime->SetText(FText::FromString(Text));

	if (Alpha >= 1.0f) {
		bPlayingClearTimeAnimation = false;
		ElapsedTime = 0.0f;
	}
}

void UEvaluationPanel::ReachedPhaseAnimation(float InDeltaTime)
{
	ElapsedTime += InDeltaTime;

	float Alpha = FMath::Clamp(ElapsedTime / AnimationDuration, 0.0f, 1.0f);

	CurrentDisplayPhase = FMath::RoundToInt(
		FMath::Lerp(0.0f, static_cast<float>(TargetPhase), Alpha)
	);

	ReachedPhase->SetText(FText::FromString(FString::FromInt(CurrentDisplayPhase)));

	if (Alpha >= 1.0f) {
		bPlayingReachedPhaseAnimation = false;
		ElapsedTime = 0.0f;
	}
}

void UEvaluationPanel::AchievementAnimation(float InDeltaTime)
{
	ElapsedTime += InDeltaTime;

	if (Achievements.IsEmpty()) {
		bPlayingAchievemtsCheckAnimation = false;
		return;
	}

	if (ElapsedTime >= AchievementClearCheckDuration) {
		ElapsedTime = 0.0f;
		TObjectPtr<UResultAchievementWidget> achievement;
		Achievements.Dequeue(achievement);

		achievement->ClearCheck(ResultData);

	}
}
