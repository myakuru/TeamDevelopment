// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Data/Result/ResultData/ResultData.h"
#include "ResultWidget.generated.h"

class UWidgetSwitcher;
class UEvaluationPanel;

/**
 * 
 */

 /** リザルト画面ページEnum */
UENUM()
enum class EResultPage : uint8
{
	Evaluation	= 0,
	Reward		= 1
};

UCLASS(Blueprintable)
class PROJECTNULL_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct()override;
	bool Initialize()override;

	/** ページを進める */
	void NextPage();

private:

	EResultPage CurrentPage = EResultPage::Evaluation;

	/** リザルトページスイッチャー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ResultSwitcher;

	/** 評価ページ */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UEvaluationPanel> EvaluationPanel;

	/** 指定したページに切り替える */
	void ShowPage(EResultPage Page);

	/** 子ウィジェットのイベントに登録するページ進行関数 */
	UFUNCTION()
	void OnChildNextPageRequested();

};
