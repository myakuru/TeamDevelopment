// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpBarWidget.h"

#include "Components/Image.h"

void UPlayerExpBarWidget::SetExp(float CurrentExp, float NextLevelExp)
{
	if (ExpImage)
	{
		CurrentExp = FMath::Clamp(CurrentExp, 0.0f, NextLevelExp);

		float ExpRatio = CurrentExp / NextLevelExp;

		UE_LOG(LogTemp, Log, TEXT("CurrentExp: %f, NextLevelExp: %f, ExpRatio: %f"), CurrentExp, NextLevelExp, ExpRatio);

		ExpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		ExpImage->SetRenderScale(FVector2D(ExpRatio, 1.0f));
	}
}