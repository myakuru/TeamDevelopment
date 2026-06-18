// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../../Data/Result/ResultData/ResultData.h"
#include "ResultAchievementWidget.generated.h"

class UTextBlock;
class UImage;
class URankConditionData;
class UBorder;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UResultAchievementWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void Initialize(URankConditionData* RankConditionData);

	void ClearCheck(const FResultData& ResultData);

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AchievementText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AchievementIcon;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BackGround;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Overlap;

	UPROPERTY(EditAnywhere)
	FLinearColor ClearColor;

	UPROPERTY(EditAnywhere)
	FLinearColor FailedColor;

	TObjectPtr<URankConditionData> RankConditionData;
	
};
