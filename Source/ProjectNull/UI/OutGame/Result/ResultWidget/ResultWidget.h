// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Data/Result/ResultData/ResultData.h"
#include "ResultWidget.generated.h"

class UCanvasPanel;
class UWidgetSwitcher;

/**
 * 
 */

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

	bool Initialize();

	void NextPage();
	void ShowPage(EResultPage Page);
	
private:

	UPROPERTY()
	EResultPage CurrentPage = EResultPage::Evaluation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWidgetSwitcher> ResultSwitcher;
};
