#include "ReturnButtonWidget.h"
#include "Kismet/GameplayStatics.h"

void UReturnButtonWidget::DoClickedButton()
{
	if (OpenLevelName.IsNone())return;

	UGameplayStatics::OpenLevel(this, OpenLevelName);
}
