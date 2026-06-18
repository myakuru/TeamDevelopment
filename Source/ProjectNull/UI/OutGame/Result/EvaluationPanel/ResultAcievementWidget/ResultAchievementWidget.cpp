// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultAchievementWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include <ProjectNull\Data\Result\RankConditionData\RankConditionData.h>
#include <ProjectNull\Data\Result\ResultData\ResultData.h>

void UResultAchievementWidget::Initialize(URankConditionData* InRankConditionData)
{
	if (!InRankConditionData)return;

	RankConditionData = InRankConditionData;

	AchievementText->SetText(RankConditionData->GetConditionText());
}

void UResultAchievementWidget::ClearCheck(const FResultData& ResultData)
{
	if (RankConditionData->IsConditionMet(ResultData)) BackGround->SetBrushColor(ClearColor);
	else BackGround->SetBrushColor(FailedColor);
}
