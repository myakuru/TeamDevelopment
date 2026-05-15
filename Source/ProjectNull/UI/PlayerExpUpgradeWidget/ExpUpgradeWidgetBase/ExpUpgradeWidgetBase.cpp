// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpUpgradeWidgetBase.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h>
#include "Input/Events.h"
#include "Input/Reply.h" 

void UExpUpgradeWidgetBase::NativeConstruct()
{
	if (UpgradeText)
	{
		UpgradeText->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	}

	UiScale = UiScaleMin;
}

void UExpUpgradeWidgetBase::ChoicesExpUpgrade()
{
	int32 Index = GetRandomTextId();

	UDataTable* LoadedTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));

	if (!LoadedTable) return;

	// 全ての行名を取得
	TArray<FName> RowNames = LoadedTable->GetRowNames();

	// ランダムで指定された行を取得
	FName RowName = RowNames[Index];

	// その行のデーターを取得
	FExpUpgradeRow* RowData = LoadedTable->FindRow<FExpUpgradeRow>(RowName, TEXT("FindData"));

	if (RowData)
	{
		if (UpgradeText)
		{
			ImageRotation();
			UpgradeText->SetText(RowData->UpgradeText);
		}
	}
}

int32 UExpUpgradeWidgetBase::GetRandomTextId()
{
	// データーテーブルのブループリント取得
	UDataTable* LoadedTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));

	if (LoadedTable)
	{
		if (!LoadedTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("DataTable is null!"));
			return 0;
		}

		TArray<FExpUpgradeRow*> AllRows;

		// 全テーブルを取得
		LoadedTable->GetAllRows<FExpUpgradeRow>(TEXT(""), AllRows);

		if (AllRows.Num() == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("DataTable has no rows!"));
			return 0;
		}

		// ランダムなインデックスを選択
		int32 RandomIndex = FMath::RandRange(0, AllRows.Num() - 1);

		return RandomIndex;
	}

	return 0;
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
