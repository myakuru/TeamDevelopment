#include "PlayerHUDWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

void UPlayerHUDWidget::NativeConstruct()
{
	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnClickedActionButton);
	}
}

void UPlayerHUDWidget::OnClickedActionButton()
{
	const FString Message = TEXT("ActionButtonがクリックされました！");
	HPText->SetText(FText::FromString(Message));
}
