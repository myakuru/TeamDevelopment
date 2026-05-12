// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectHUDWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UStageSelectHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

};
