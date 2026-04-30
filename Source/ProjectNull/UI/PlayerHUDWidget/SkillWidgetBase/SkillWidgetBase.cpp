// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidgetBase.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USkillWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillImage)
	{
		CooldownMaterial = SkillImage->GetDynamicMaterial();
	}
}

void USkillWidgetBase::UpdateRotationImage(float CooldownTime)
{
	if(CooldownMaterial)
	{
		// 角度はマテリアル側で0から1の範囲にしてる。
		CooldownTime = FMath::Clamp(CooldownTime, 0.0f, 1.0f);

		// マテリアル側に回転角度を渡す
		CooldownMaterial->SetScalarParameterValue(TEXT("RotationAngle"), CooldownTime);
	}
}

void USkillWidgetBase::UpdateCooldownText(float CooldownTime)
{
	if (CooldownText)
	{
		// クールダウン時間を秒数で表示する（小数点以下切り捨て）
		FString CooldownString = FString::Printf(TEXT("%.1f"), CooldownTime);

		CooldownText->SetText(FText::FromString(CooldownString));
	}
}
