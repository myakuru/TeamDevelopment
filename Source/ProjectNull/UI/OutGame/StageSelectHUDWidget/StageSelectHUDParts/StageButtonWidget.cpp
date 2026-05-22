#include "StageButtonWidget.h"
#include "Components/Button.h"

void UStageButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StageButton)
	{
		StageButton->bIsVariable = true;

		StageButton->OnClicked.AddUniqueDynamic(this, &UStageButtonWidget::OnClickedStageButton);
		StageButton->OnHovered.AddUniqueDynamic(this, &UStageButtonWidget::OnHoveredStageButton);
	}
}

void UStageButtonWidget::OnClickedStageButton()
{
	if (!OnClicked.IsBound() || !bUnlocked)return;
	OnClicked.Broadcast(StageIndex);
}

void UStageButtonWidget::OnHoveredStageButton()
{
	if (!OnHovered.IsBound() || !bUnlocked)return;
	OnHovered.Broadcast(StageIndex);
}

void UStageButtonWidget::Setup(int32 InStageIndex,bool bInUnlocked)
{
	StageIndex	= InStageIndex;
	bUnlocked	= bInUnlocked;
}
