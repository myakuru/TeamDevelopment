#include "ButtonBaseWidget.h"
#include "Components/Button.h"

void UButtonBaseWidget::NativeConstruct()
{
	if (!Button)return;

	Button->bIsVariable = true;

	Button->OnHovered.AddDynamic(this, &UButtonBaseWidget::OnHoveredButton);
	Button->OnClicked.AddDynamic(this, &UButtonBaseWidget::OnClickedButton);
}

void UButtonBaseWidget::OnHoveredButton()
{
	DoHoveredButton();
}

void UButtonBaseWidget::DoHoveredButton()
{
	if (!OnHovered.IsBound())return;
	OnHovered.Broadcast();
}

void UButtonBaseWidget::OnClickedButton()
{
	DoClickedButton();
}

void UButtonBaseWidget::DoClickedButton()
{
	if (!OnClicked.IsBound())return;
	OnClicked.Broadcast();
}
