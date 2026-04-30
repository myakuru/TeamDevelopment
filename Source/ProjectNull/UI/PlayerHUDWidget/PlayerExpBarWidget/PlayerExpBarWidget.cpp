// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpBarWidget.h"

#include "Components/Image.h"

void UPlayerExpBarWidget::SetExp(int32 CurrentExp, int32 NextLevelExp)
{
	if (ExpImage)
	{
		float ExpRatio = static_cast<float>(CurrentExp) / static_cast<float>(NextLevelExp);
		ExpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		ExpImage->SetRenderScale(FVector2D(ExpRatio, 1.0f));
	}
}
