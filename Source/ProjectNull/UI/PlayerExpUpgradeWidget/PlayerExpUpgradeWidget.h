// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerExpUpgradeWidget.generated.h"

class USuperGameInstance;
class UExpUpgradeWidgetBase;
class USuperGameInstance;
class UPlayerRuntimeData;

class UImage;

/**
 * 経験値がレベルアップしたら呼ばれるクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerExpUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** プレイヤー側が呼び出す */
	void OpenUpgradeWidget();

	// 左ボタン押下を処理する
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;


protected:

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	/** 強化画面を開く */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void OpenWidget();

	/** 強化画面を閉じる */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void CloseWidget();

	void BackgroundImageFadeIn();

	void InitUpgradeWidget();

	/** 背景の黒い画像 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> BackgroundImage;

	/** ゲームインスタンスへの参照 */
	TObjectPtr<USuperGameInstance> GameInstance;

	/** プレイヤーのランタイムデータへの参照 */
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;

	/** 強化画面0のウィジェット */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UExpUpgradeWidgetBase> UpgradeWidget_0;

	/** 強化画面1のウィジェット */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UExpUpgradeWidgetBase> UpgradeWidget_1;

	/** 強化画面2のウィジェット */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UExpUpgradeWidgetBase> UpgradeWidget_2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	bool bIsUpgradeWidgetUse = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float BackgroundFadeInDuration = 1.0f; // フェードインの時間

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float BackgroundFadeInAlpha = 0.0f; // 背景の黒い画像のアルファ値
};
