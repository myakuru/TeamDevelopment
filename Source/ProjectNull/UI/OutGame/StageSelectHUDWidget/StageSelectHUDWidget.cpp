// Fill out your copyright notice in the Description page of Project Settings.


#include "StageSelectHUDWidget.h"
#include "Components/TextBlock.h"

void UStageSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StageNameText)
	{
		StageNameText->SetText(FText::FromString(TEXT("Select")));
	}
}
