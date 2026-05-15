#include "StageSelectHUDWidget.h"
#include "Components/TextBlock.h"
#include"Components/VerticalBox.h"

//ステージのデータアセット
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
//ステージボタンUI
#include<ProjectNull/UI/OutGame/StageSelectHUDWidget/StageSelectHUDParts/StageButtonWidget.h>

void UStageSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (StageNameText)
	{
		StageNameText->SetText(FText::FromString(TEXT("StageSelect")));
	}

	//for (int32 i = 0; i < StageList.Num(); i++)
	//{
	//	auto* ButtonWidget =
	//		CreateWidget<UStageButtonWidget>(this, UStageButtonWidget::StaticClass());

	//	ButtonWidget->Setup(i);

	//	StageListBox->AddChild(ButtonWidget);
	//}
}