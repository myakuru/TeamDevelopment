// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultAchievementWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UResultAchievementWidget : public UUserWidget
{
	GENERATED_BODY()

public:

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AchievementText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AchievementIcon;
	
};
