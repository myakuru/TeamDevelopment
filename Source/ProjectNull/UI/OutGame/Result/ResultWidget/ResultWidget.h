// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UTextBlock;
class UVerticalBox;
class UResultAchievementWidget;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct()override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearTime;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReachedPhase;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> AchieveList;

	TArray<TObjectPtr<UResultAchievementWidget>> Achievements;

};
