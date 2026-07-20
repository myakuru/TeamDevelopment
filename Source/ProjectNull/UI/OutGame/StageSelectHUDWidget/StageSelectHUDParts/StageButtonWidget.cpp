#include "StageButtonWidget.h"
#include "Components/Button.h"

void UStageButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();


	//Baseが持ているホバー・クリックデリゲートに、
	//ステージボタン用のデリゲートを登録する
	OnHovered.AddUniqueDynamic
	(this, &UStageButtonWidget::DoHoveredStageButton);

	OnClicked.AddUniqueDynamic
	(this, &UStageButtonWidget::DoClickedStageButton);
}

void UStageButtonWidget::DoHoveredButton()
{
	if (!bUnlocked)return;
	Super::DoHoveredButton();
}

void UStageButtonWidget::DoClickedButton()
{
	if (!bUnlocked)return;
	Super::DoClickedButton();
}

void UStageButtonWidget::DoHoveredStageButton()
{
	if (!OnHoveredStage.IsBound() || !bUnlocked)return;
	OnHoveredStage.Broadcast(StageIndex);
}

void UStageButtonWidget::DoClickedStageButton()
{
	if (!OnClickedStage.IsBound() || !bUnlocked)return;
	OnClickedStage.Broadcast(StageIndex);
}

void UStageButtonWidget::Setup(int32 InStageIndex,bool bInUnlocked)
{
	StageIndex	= InStageIndex;
	bUnlocked	= bInUnlocked;
}
