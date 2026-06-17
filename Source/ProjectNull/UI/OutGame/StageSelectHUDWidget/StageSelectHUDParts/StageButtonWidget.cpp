#include "StageButtonWidget.h"
#include "Components/Button.h"

void UStageButtonWidget::DoHoveredButton()
{
	if (!OnHovered.IsBound() || !bUnlocked)return;
	OnHovered.Broadcast(StageIndex);
}

void UStageButtonWidget::DoClickedButton()
{
	if (!OnClicked.IsBound() || !bUnlocked)return;
	OnClicked.Broadcast(StageIndex);
}

void UStageButtonWidget::Setup(int32 InStageIndex,bool bInUnlocked)
{
	StageIndex	= InStageIndex;
	bUnlocked	= bInUnlocked;
}
