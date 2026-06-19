// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../../../../Data/Result/ResultData/ResultData.h"
#include "EvaluationPanel.generated.h"

class UTextBlock;
class UVerticalBox;
class UButton;
class UResultAchievementWidget;

/** リザルトページ進行デリゲート宣言マクロ */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextPageRequested);

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UEvaluationPanel : public UUserWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct()override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime)override;

	/** リザルトページ進行デリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnNextPageRequested OnNextPageRequested;

private:

	/** ボタンクリックに登録する関数 */
	UFUNCTION()
	void OnToRewardButtonClicked();

	/** クリア時間UI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ClearTime;

	/** 進行フェーズUI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ReachedPhase;

	/** クリア条件リストUI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UVerticalBox> AchieveList;

	/** 報酬画面移行ボタンUI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ToRewardButton;

	/** クリア条件リストアイテムUI生成 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UResultAchievementWidget> ResultAchievementWidgetClass;

	/** クリア条件リスト(クリアチェックアニメーション用にQueue配列) */
	TQueue<TObjectPtr<UResultAchievementWidget>> Achievements;

	/** クリアデータ */
	FResultData ResultData;

	/** アニメーション再生フラグ */
	bool bPlayingClearTimeAnimation = false;
	bool bPlayingReachedPhaseAnimation = false;
	bool bPlayingAchievemtsCheckAnimation = false;

	/** 表示しているクリア時間 */
	float CurrentDisplayTime = 0.0f;

	/** 表示している到達フェーズ */
	int32 CurrentDisplayPhase = 0;

	/** カウントアップアニメーションにかかる時間 */
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 1.0f;

	/** クリア条件チェックにかかる時間 */
	UPROPERTY(EditAnywhere)
	float AchievementClearCheckDuration = 1.0f;

	/** タイマー */
	float ElapsedTime = 0.0f;

	/** クリア時間カウントアップアニメーション */
	void ClearTimeAnimation(float InDeltaTime);

	/** 到達フェーズカウントアップアニメーション */
	void ReachedPhaseAnimation(float InDeltaTime);

	/** クリア条件チェックアニメーション */
	void AchievementAnimation(float InDeltaTime);

	
};
