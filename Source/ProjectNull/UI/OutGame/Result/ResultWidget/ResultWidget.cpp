// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Result/ResultManager/ResultManager.h>
#include <ProjectNull/Weapon/Instance/WeaponInstance.h>
#include <ProjectNull/UI/OutGame/Result/EvaluationPanel/EvaluationPanel.h>
#include <ProjectNull/UI/OutGame/Result/RewardPanel/RewardPanel.h>

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 評価ページのイベントにページ進行関数登録
	if (EvaluationPanel) {
		EvaluationPanel->OnNextPageRequested.AddUniqueDynamic(
			this,
			&UResultWidget::OnChildNextPageRequested);
	}

	if (RewardPanel) {
		RewardPanel->OnNextPageRequested.AddUniqueDynamic(
			this,
			&UResultWidget::OnChildNextPageRequested
		);
	}

}

bool UResultWidget::Initialize()
{
	Super::Initialize();

	//マウスカーソル表示
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();

	if (!PC)return false;

	PC->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(
		TakeWidget()
	);

	PC->SetInputMode(InputMode);

	// 評価ページで初期化
	ShowPage(EResultPage::Evaluation);

	return false;
}

void UResultWidget::NextPage()
{

	USuperGameInstance* gameInstance = GetGameInstance<USuperGameInstance>();
	if (!gameInstance)UGameplayStatics::OpenLevel(this, "StageSelectLevel");

	UResultManager* resultManager = gameInstance->GetResultManager();
	if (!resultManager)UGameplayStatics::OpenLevel(this, "StageSelectLevel");

	switch (CurrentPage) {
	case EResultPage::Evaluation:
		if (resultManager->GetRewardWeapons().IsEmpty()) {
			UGameplayStatics::OpenLevel(this, "StageSelectLevel");
			break;
		}

		ShowPage(EResultPage::Reward);
		if (RewardPanel)RewardPanel->Initialize();
		break;

	case EResultPage::Reward:
		UGameplayStatics::OpenLevel(this, "StageSelectLevel");
		break;
	}
}

void UResultWidget::ShowPage(EResultPage Page)
{
	CurrentPage = Page;
	ResultSwitcher->SetActiveWidgetIndex(
		static_cast<int32>(Page)
	);
	
}

void UResultWidget::OnChildNextPageRequested()
{
	NextPage();
}
