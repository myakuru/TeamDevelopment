// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultAchievementWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "Components/Image.h"
#include <ProjectNull\Data\Result\RankConditionData\RankConditionData.h>
#include <ProjectNull\Data\Result\ResultData\ResultData.h>

void UResultAchievementWidget::Initialize(URankConditionData* InRankConditionData)
{
	if (!InRankConditionData)return;

	RankConditionData = InRankConditionData;

	if (ClearIcon)ClearIcon->SetVisibility(ESlateVisibility::Hidden);
	if (FailedIcon)FailedIcon->SetVisibility(ESlateVisibility::Hidden);

	AchievementText->SetText(RankConditionData->GetConditionText());
}

void UResultAchievementWidget::ClearCheck(const FResultData& ResultData)
{
	// 達成条件が達成されているかによって背景色を変更する
	if (RankConditionData->IsConditionMet(ResultData)) {
		if (ClearIcon)ClearIcon->SetVisibility(ESlateVisibility::Visible);
		BackGround->SetBrushColor(ClearColor);
	}
	else {
		if (FailedIcon)FailedIcon->SetVisibility(ESlateVisibility::Visible);
		BackGround->SetBrushColor(FailedColor);
	}
}
