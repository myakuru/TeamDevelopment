// Fill out your copyright notice in the Description page of Project Settings.


#include "GearChangeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGearChangeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ギアチェンジエネルギーのマテリアルを取得
	if (GearChangeImage)
	{
		GearChangeMaterial = GearChangeImage->GetDynamicMaterial();
	}

	SetIsFocusable(false);
}

void UGearChangeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	static float ChargeAmount = 0.0f;

	ChargeAmount += InDeltaTime * 5.0f;

	SetGearChangeEnergy(ChargeAmount);

}

void UGearChangeWidget::SetGearChangeEnergy(float ChargeAmount)
{
	if (GearChangeMaterial)
	{
		GearChangeMaterial->SetScalarParameterValue(TEXT("ChargeAmount"), ChargeAmount);
	}
	if (GearChangeText)
	{
		// 表示する値を0から1000の範囲に制限
		ChargeAmount = std::clamp(ChargeAmount, MinChargeAmount, MaxChargeAmount);

		// ~%と表示される
		FString ChargeText = FString::Printf(TEXT("%.0f"), ChargeAmount);

		GearChangeText->SetText(FText::FromString(ChargeText));
	}
}
