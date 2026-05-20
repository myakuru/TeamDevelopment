// Fill out your copyright notice in the Description page of Project Settings.

#include "ExpUpgradeWidgetBase.h"
#include <Components/TextBlock.h>
#include <Components/Image.h>
#include <ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h>
#include "Input/Events.h"
#include "Input/Reply.h"

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

void UExpUpgradeWidgetBase::ChoicesExpUpgrade()
{
	UDataTable* Table = GetExpUpgradeTable();
	if (!Table) return;

	// 全行名を取得（行番号でランダム選択するために配列インデックスを扱う）
	TArray<FName> RowNames = Table->GetRowNames();
	if (RowNames.Num() == 0)
	{
		if (UpgradeText) UpgradeText->SetText(FText::GetEmpty());
		SelectedRowName = NAME_None;
		SelectedRowData = nullptr;
		return;
	}

	// 候補行を「現在レベルが Max 未満」のものに絞る
	TArray<int32> CandidateIndices;
	for (int32 i = 0; i < RowNames.Num(); ++i)
	{
		const FExpUpgradeRow* RowData = Table->FindRow<FExpUpgradeRow>(RowNames[i], TEXT("ChoicesExpUpgrade"));
		if (!RowData) continue;

		int32 MaxLevel = FMath::Max(0, RowData->UpgradeTexts.Num() - 1);
		int32 CurrentLevel = PlayerUpgradeLevels.Contains(RowNames[i]) ? PlayerUpgradeLevels[RowNames[i]] : 0;

		// Max に到達している行は候補から外す
		if (CurrentLevel >= MaxLevel) continue;

		CandidateIndices.Add(i);
	}

	// 候補がなければクリア
	if (CandidateIndices.Num() == 0)
	{
		if (UpgradeText) UpgradeText->SetText(FText::GetEmpty());
		SelectedRowName = NAME_None;
		SelectedRowData = nullptr;
		return;
	}

	// 行番号（配列インデックス）でランダム選択
	int32 Pick = CandidateIndices[FMath::RandRange(0, CandidateIndices.Num() - 1)];
	FName RowName = RowNames[Pick];

	// 選択を記録（名前と行データをキャッシュ）
	SelectedRowName = RowName;
	SelectedRowData = Table->FindRow<FExpUpgradeRow>(SelectedRowName, TEXT("ChoicesExpUpgrade"));

	if (SelectedRowData && UpgradeText)
	{
		ImageRotation();
		// キャッシュされている PlayerUpgradeLevels のインデックス（初回は 0）で表示する
		ShowTextForCurrentLevel(SelectedRowName, SelectedRowData);
	}
}

void UExpUpgradeWidgetBase::OnUpgradeClickedByName(FName ClickedRowName)
{
	if (ClickedRowName == NAME_None) return;

	UDataTable* Table = GetExpUpgradeTable();
	if (!Table) return;

	// 優先してキャッシュされた RowData を使う
	const FExpUpgradeRow* RowData = (SelectedRowName == ClickedRowName && SelectedRowData) ? SelectedRowData : Table->FindRow<FExpUpgradeRow>(ClickedRowName, TEXT("OnUpgradeClickedByName"));
	if (!RowData)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ExpUpg] Row not found: %s"), *ClickedRowName.ToString());
		return;
	}

	// 現状のインデックス（level）と最大インデックスを取得
	int32& LevelRef = PlayerUpgradeLevels.FindOrAdd(ClickedRowName, 0);
	int32 CurrentBefore = LevelRef;
	int32 MaxLevel = FMath::Max(0, RowData->UpgradeTexts.Num() - 1);

	UE_LOG(LogTemp, Log, TEXT("[ExpUpg] Clicked: %s Before=%d Max=%d"), *ClickedRowName.ToString(), CurrentBefore, MaxLevel);

	// 配列の範囲内でのみインクリメント（Max到達後は増やさない）
	if (CurrentBefore < MaxLevel)
	{
		++LevelRef;
		UE_LOG(LogTemp, Log, TEXT("[ExpUpg] Incremented %s -> %d"), *ClickedRowName.ToString(), LevelRef);
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("[ExpUpg] Already at Max for %s"), *ClickedRowName.ToString());
	}

	// 更新後のインデックスに応じて表示を更新
	ShowTextForCurrentLevel(ClickedRowName, RowData);
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

void UExpUpgradeWidgetBase::OnUpgradeClicked()
{
	// 互換呼び出し（SelectedRowName を使う）
	if (SelectedRowName == NAME_None) return;
	OnUpgradeClickedByName(SelectedRowName);
}

void UExpUpgradeWidgetBase::ShowTextForCurrentLevel(FName RowName, const FExpUpgradeRow* RowData)
{
	if (!RowData || !UpgradeText) return;

	int32 CurrentLevel = PlayerUpgradeLevels.Contains(RowName) ? PlayerUpgradeLevels[RowName] : 0;
	int32 MaxLevel = FMath::Max(0, RowData->UpgradeTexts.Num() - 1);

	// 最大到達時の表示
	if (CurrentLevel >= MaxLevel)
	{
		UpgradeText->SetText(FText::FromString(TEXT("Max")));
		return;
	}

	// 安全に配列インデックスを参照
	if (RowData->UpgradeTexts.IsValidIndex(CurrentLevel))
	{
		const FText& Text = RowData->UpgradeTexts[CurrentLevel];
		if (!Text.IsEmpty())
		{
			UpgradeText->SetText(Text);
			return;
		}
	}

	// 該当がなければクリア
	UpgradeText->SetText(FText::GetEmpty());

}
