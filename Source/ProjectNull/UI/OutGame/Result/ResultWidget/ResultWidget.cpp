// Fill out your copyright notice in the Description page of Project Settings.


#include "ResultWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/WidgetSwitcher.h"

bool UResultWidget::Initialize()
{
	Super::Initialize();

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
