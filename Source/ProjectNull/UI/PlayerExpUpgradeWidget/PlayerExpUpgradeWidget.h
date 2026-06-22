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

/** 1レベル分の強化情報 */
USTRUCT(BlueprintType)
struct FValidUpgradeInfo
{
	GENERATED_BODY()

	FName RowName;
	FText Description;
	float Multiplier;
	FName CurrentLevel;
};

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

	void ChoicesExpUpgrade();

	/** 強化画面を開く */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void OpenWidget();

	/** 強化画面を閉じる */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void CloseWidget();

	void BackgroundImageFadeIn();

	void InitUpgradeWidget();

	void SetAttackMultiplier(float Multiplier);

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

	/** フェードインの時間 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float BackgroundFadeInDuration = 1.0f;

	/** 背景の黒い画像のアルファ値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float BackgroundFadeInAlpha = 0.0f;

	/** 背景の黒い画像のアルファ値の最大値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Upgrade")
	float BackgroundFadeInAlphaMax = 0.0f;

	/** プレイヤー側で保持する各行ごとの現在レベル（デフォルトは 0） */
	TMap<FName, int32> PlayerUpgradeLevels;

	/** すでに表示した行を記録するセット（同じ行を複数回表示しないようにするため） */
	TSet<FName> ShownUpgradeRows;

	/** DataTable をキャッシュしておく（毎回ロードしない） */
	UPROPERTY()
	UDataTable* CachedExpUpgradeTable = nullptr;

	bool bIsUpgradeWidgetFilledArray = true;

	bool bIsUpgradeWidgetOpen = false;

	/** 現在表示中の行名（ChoicesExpUpgrade でセット） */
	FName SelectedRowName = NAME_None;

	/** DataTable のロード（内部でキャッシュ） */
	UDataTable* GetExpUpgradeTable();

private:
	// 説明文の数
	TArray<FValidUpgradeInfo> ValidUpgrades;
};
