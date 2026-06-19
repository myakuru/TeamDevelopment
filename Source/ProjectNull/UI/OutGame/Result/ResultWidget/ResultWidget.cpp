// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"
#include <ProjectNull/UI/OutGame/Result/EvaluationPanel/EvaluationPanel.h>

void UResultWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 評価ページのイベントにページ進行関数登録
	if (EvaluationPanel) {
		EvaluationPanel->OnNextPageRequested.AddDynamic(
			this,
			&UResultWidget::OnChildNextPageRequested);
	}
}

bool UResultWidget::Initialize()
{
	Super::Initialize();

	// 評価ページで初期化
	ShowPage(EResultPage::Evaluation);

	return false;
}

void UResultWidget::NextPage()
{
	switch (CurrentPage) {
	case EResultPage::Evaluation:
		ShowPage(EResultPage::Reward);
		break;

	case EResultPage::Reward:
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
