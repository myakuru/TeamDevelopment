#include "StageSelectHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Kismet/GameplayStatics.h"

//ステージのデータアセット
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
//ステージボタンUI
#include<ProjectNull/UI/OutGame/StageSelectHUDWidget/StageSelectHUDParts/StageButtonWidget.h>

void UStageSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StageDataList && StageNameText && StageDataList->StageData.Num() > 0)
	{
		StageNameText->SetText(StageDataList->StageData[0].StageName);
	}

	CreateStageButtons();
}

void UStageSelectHUDWidget::CreateStageButtons()
{
	if (!StageDataList)return;

	//ステージのデータ数だけステージボタンを生成していく
	for (int32 i = 0; i < StageDataList->StageData.Num(); i++)
	{
		auto* ButtonWidget =
			CreateWidget<UStageButtonWidget>(this, StageButtonWidgetClass);

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("CreateButtonWidgetClass = %s"),
			*ButtonWidget->GetClass()->GetName()
		);

		if (!ButtonWidget)continue;

		//1から始めるようにセットアップ
		ButtonWidget->Setup(i + 1);

		//デリゲートのバインド
		ButtonWidget->OnHovered.AddUniqueDynamic(
			this, &UStageSelectHUDWidget::OnHoveredStageButton);

		ButtonWidget->OnClicked.AddUniqueDynamic(
			this, &UStageSelectHUDWidget::OnClickedStageButton);

		UCanvasPanelSlot* CanvasSlot =
			StageCanvas->AddChildToCanvas(ButtonWidget);

		if (!CanvasSlot)continue;

		CanvasSlot->SetPosition(FVector2D(100.0f, 100.0f * i));

		CanvasSlot->SetSize(FVector2D(300.0f, 80.0f));
	}
}

void UStageSelectHUDWidget::OnClickedStageButton(int32 InStageIndex)
{
	if (!StageDataList || StageDataList->StageData.Num() == 0)return;

	UGameplayStatics::OpenLevel(this, FName(StageDataList->StageData[InStageIndex - 1].LevelName));
}

void UStageSelectHUDWidget::OnHoveredStageButton(int32 InStageIndex)
{
	CurrentSelectedStageIndex = InStageIndex;

	if (!StageNameText)return;
	if (!StageDataList || StageDataList->StageData.Num() == 0)return;

	StageNameText->SetText(StageDataList->StageData[InStageIndex - 1].StageName);
}
