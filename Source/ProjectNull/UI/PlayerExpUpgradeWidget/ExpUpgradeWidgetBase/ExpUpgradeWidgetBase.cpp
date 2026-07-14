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
		// 自動的に改行してもらう
		UpgradeText->SetWrappingPolicy(ETextWrappingPolicy::AllowPerCharacterWrapping);
	}

	UIImageParameter.UiScale = UIImageParameter.UiScaleMin;
	UITextParameter.UiScale = UITextParameter.UiScaleMin;
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

void UExpUpgradeWidgetBase::UpdateScale()
{
	if (!UpgradeImage) return;
	if (!UpgradeText)  return;

	// ホバー中は拡大サイズ、そうでなければ通常サイズを目標にする
	const FVector2D TargetImageScale = bIsMouseOver ? UIImageParameter.UiScaleHover : UIImageParameter.UiScaleMax;
	
	const FVector2D TargetTextScale = bIsMouseOver ? UITextParameter.UiScaleHover : UITextParameter.UiScaleMax;

	// 毎フレーム目標スケールへ補間する。
	UIImageParameter.UiScale = FMath::Vector2DInterpTo(UIImageParameter.UiScale, TargetImageScale, GetWorld()->GetDeltaSeconds(), ScaleInterpSpeed);

	UITextParameter.UiScale = FMath::Vector2DInterpTo(UITextParameter.UiScale, TargetTextScale, GetWorld()->GetDeltaSeconds(), ScaleInterpSpeed);
	
	// カードの大きさを変更
	UpgradeImage->SetRenderScale(UIImageParameter.UiScale);
	
	// テキストの大きさも変更
	UpgradeText->SetRenderScale(UITextParameter.UiScale);
}

void UExpUpgradeWidgetBase::InitExpUpgradeWidget()
{
	// 出現アニメーションを最初から再生できるように初期スケールへ戻す
	UIImageParameter.UiScale = UIImageParameter.UiScaleMin;
	UITextParameter.UiScale = UITextParameter.UiScaleMin;
	bIsMouseOver = false;
}

void UExpUpgradeWidgetBase::NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseEnter(InGeometry, InMouseEvent);

	// 状態を記録するだけ。実際のスケール変化は UpdateScale() が毎フレーム行う
	bIsMouseOver = true;
}

void UExpUpgradeWidgetBase::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);

	bIsMouseOver = false;
}

void UExpUpgradeWidgetBase::SetDescriptionText(const FText& Description)
{
	if (UpgradeText)
	{
		UpgradeText->SetText(Description);
	}
}
