// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerHpBarWidget.h"

#include "Components/Image.h"

void UPlayerHpBarWidget::SetHp(int32 CurrentHp, int32 MaxHp)
{
	if (HpImage)
	{
		float HpRatio = static_cast<float>(CurrentHp) / static_cast<float>(MaxHp);

		HpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		HpImage->SetRenderScale(FVector2D(HpRatio, 1.0f));
	}
}
