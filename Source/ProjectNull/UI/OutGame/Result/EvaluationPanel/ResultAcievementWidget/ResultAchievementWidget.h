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

	/** このUIにセットされた達成条件が満たされているかチェックして合否アニメーションを行う */
	void ClearCheck(const FResultData& ResultData);

private:

	/** 達成条件説明文 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> AchievementText;

	/** 達成条件アイコン */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> AchievementIcon;

	/** 背景 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BackGround;

	/** ラップ */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Overlap;

	/** 条件をクリアしていた時の背景色 */
	UPROPERTY(EditAnywhere)
	FLinearColor ClearColor;

	/** 条件を失敗していた時の背景色 */
	UPROPERTY(EditAnywhere)
	FLinearColor FailedColor;

	/** 達成条件データアセット */
	TObjectPtr<URankConditionData> RankConditionData;
	
};
