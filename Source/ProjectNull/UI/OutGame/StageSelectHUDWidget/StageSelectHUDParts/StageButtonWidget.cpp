#include "StageButtonWidget.h"
#include "Components/Button.h"

#include "Kismet/GameplayStatics.h"

void UStageButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StageButton)
	{
		StageButton->OnClicked.AddUniqueDynamic(this, &UStageButtonWidget::OnClickedStageButton);
	}
}

void UStageButtonWidget::OnClickedStageButton()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(*FString::Printf(TEXT("Stage%dMapVer4"), StageIndex)));
}

void UStageButtonWidget::Setup(int32 InStageIndex)
{
	StageIndex = InStageIndex;
}
