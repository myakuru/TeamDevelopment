// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	MaxUiScale = SkillImage->RenderTransform.Scale;

	if (SkillImage)
	{
		CooldownMaterial = SkillImage->GetDynamicMaterial();
	}
}

void USkillWidgetBase::SetUIScale(float CooldownTime)
{
	if (CooldownTime <= 0.0f)
	{
		UIScale -= UIScaleDecreaseRate * GetWorld()->GetDeltaSeconds();

		if (SkillImage && CooldownText)
		{
			if (UIScale >= 0.0f)
			{
				SkillImage->SetRenderScale(FVector2D(UIScale, UIScale));
				CooldownText->SetRenderScale(FVector2D(UIScale, UIScale));
			}
			else
			{
				UIScale = 0.0f;
			}
		}
	}
}

void USkillWidgetBase::ShowUI(bool bShow)
{
	if (!bShow)
	{
		SkillImage->SetVisibility(ESlateVisibility::Hidden);
		CooldownText->SetVisibility(ESlateVisibility::Hidden);
	}
	else
	{
		SkillImage->SetVisibility(ESlateVisibility::Visible);
		CooldownText->SetVisibility(ESlateVisibility::Visible);
	}
}

void USkillWidgetBase::UpdateRotationImage(float CooldownTime, float MaxCooldownTime)
{
	if(CooldownMaterial)
	{
		if (float Ratio = MaxCooldownTime > 0.0f)
		{
			Ratio = FMath::Clamp(CooldownTime / MaxCooldownTime, 0.0f, 1.0f);

			// マテリアル側に 0〜1 の値を渡す
			CooldownMaterial->SetScalarParameterValue(TEXT("RotationAngle"), Ratio);
		}
		else
		{
			Ratio = 0.0f;
			CooldownMaterial->SetScalarParameterValue(TEXT("RotationAngle"), Ratio);
			return;
		}
	}
}

void USkillWidgetBase::UpdateCooldownText(float CooldownTime)
{
	if (CooldownText)
	{
		// クールダウン時間を秒数で表示する（小数点以下切り捨て）
		FString CooldownString = FString::Printf(TEXT("%.1f"), CooldownTime);

		CooldownText->SetText(FText::FromString(CooldownString));

		if (CooldownTime > 0.0f)
		{
			ResetUi();
		}
		
		// クールダウン時間に応じてUIの拡大率を変更する
		SetUIScale(CooldownTime);

		UIScale <= 0.0f ? ShowUI(false) : ShowUI(true);
		
	}
}

void USkillWidgetBase::ResetUi()
{
	UIScale = MaxUiScale.X; // アイコンの拡大率をリセット

	SkillImage->SetRenderScale(MaxUiScale);
	CooldownText->SetRenderScale(MaxUiScale);

	ShowUI(true);
}
