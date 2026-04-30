// Fill out your copyright notice in the Description page of Project Settings.


#include "GearChangeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/CharacterParameterData.h>

void UGearChangeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// ギアチェンジエネルギーのマテリアルを取得
	if (GearChangeImage)
	{
		GearChangeMaterial = GearChangeImage->GetDynamicMaterial();
	}
}

void UGearChangeWidget::SetGearChangeEnergy(float ChargeAmount)
{
	if (GearChangeMaterial)
	{
		GearChangeMaterial->SetScalarParameterValue(TEXT("ChargeAmount"), ChargeAmount);
	}

	if (GearChangeText)
	{
		// ~%と表示される
		FString ChargeText = FString::Printf(TEXT("%.0f"), ChargeAmount);

		GearChangeText->SetText(FText::FromString(ChargeText));
	}
}
