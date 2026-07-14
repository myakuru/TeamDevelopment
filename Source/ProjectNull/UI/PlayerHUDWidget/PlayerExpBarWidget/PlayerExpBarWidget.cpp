// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpBarWidget.h"

#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"

void UPlayerExpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ExpRainbowImage)
	{
		// ExpRainbowImage のブラシに設定したマテリアルから MID を取得
		CooldownMaterial = ExpRainbowImage->GetDynamicMaterial();

		// 初期状態は非表示（経験値マックスになったら表示する）
		ExpRainbowImage->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UPlayerExpBarWidget::SetExp(float CurrentExp, float NextLevelExp)
{
	if (ExpImage)
	{
		CurrentExp = FMath::Clamp(CurrentExp, 0.0f, NextLevelExp);

		ExpRatio = CurrentExp / NextLevelExp;
		
		ExpRatio = FMath::Clamp(ExpRatio, 0.0f, 1.0f);

		UE_LOG(LogTemp, Log, TEXT("CurrentExp: %f, NextLevelExp: %f, ExpRatio: %f"), CurrentExp, NextLevelExp, ExpRatio);

		ExpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		ExpImage->SetRenderScale(FVector2D(ExpRatio, 1.0f));
	}
}

void UPlayerExpBarWidget::SetRainbowVisible(bool bVisible)
{
	if (!ExpRainbowImage)
	{
		return;
	}

	ExpRainbowImage->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);

	if (CooldownMaterial)
	{
		// マテリアル側に公開したスカラーパラメータで光り方を制御する
		CooldownMaterial->SetScalarParameterValue(TEXT("Glow"), bVisible ? 1.0f : 0.0f);
	}
}
