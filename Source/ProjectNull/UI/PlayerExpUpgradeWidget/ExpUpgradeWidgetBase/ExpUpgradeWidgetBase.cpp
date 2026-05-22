// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpUpgradeWidgetBase.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h>
#include "Input/Events.h"
#include "Input/Reply.h"
#include "ProjectNull/GameInstance/SuperGameInstance.h"
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

void UExpUpgradeWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (UpgradeText)
	{
		UpgradeText->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	}

	UiScale = UiScaleMin;
}

UDataTable* UExpUpgradeWidgetBase::GetExpUpgradeTable()
{
	if (CachedExpUpgradeTable) return CachedExpUpgradeTable;

	CachedExpUpgradeTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));
	if (!CachedExpUpgradeTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DT_ExpUpgrade がロードできません"));
	}
	return CachedExpUpgradeTable;
}

void UExpUpgradeWidgetBase::ImageRotation()
{
	if (UpgradeImage)
	{
		if (UiScale.X <= UiScaleMax.X)
		{
			UiScale.X += GetWorld()->GetDeltaSeconds() * UiScaleSpeed.X;

			UpgradeImage->SetRenderScale(UiScale);
		}

		if (UiScale.Y >= UiScaleMax.Y)
		{
			UiScale.Y -= GetWorld()->GetDeltaSeconds() * UiScaleSpeed.Y;

			UpgradeImage->SetRenderScale(UiScale);
		}
	}
}

void UExpUpgradeWidgetBase::InitExpUpgradeWidget()
{
	if(UiScale.X >= UiScaleMax.X)
	{
		UiScale = UiScaleMin;
	}
}

void UExpUpgradeWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	bIsMouseOver = true;

	if (UpgradeImage)
	{
		if (UiScale.X <= UiScaleMax.X) return;

		if (UiScale.X <= UiScaleMax.X + 0.5f)
		{
			UiScale.X += GetWorld()->GetDeltaSeconds() * 10.0f;
		}
		if (UiScale.Y <= UiScaleMax.Y + 1.0f)
		{
			UiScale.Y += GetWorld()->GetDeltaSeconds() * 10.0f;
		}

		UpgradeImage->SetRenderScale(UiScale);
	}
}

void UExpUpgradeWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsMouseOver = false;

	if (UpgradeImage)
	{
		if (UiScale.X <= UiScaleMax.X) return;

		if (UiScale.X >= UiScaleMax.X)
		{
			UiScale.X -= GetWorld()->GetDeltaSeconds() * 10.0f;
		}
		if (UiScale.Y >= UiScaleMax.Y)
		{
			UiScale.Y -= GetWorld()->GetDeltaSeconds() * 10.0f;
		}
		UpgradeImage->SetRenderScale(UiScale);
	}
}

void UExpUpgradeWidgetBase::SetDescriptionText(const FText& Description)
{
	if (UpgradeText)
	{
		UpgradeText->SetText(Description);
	}
}
