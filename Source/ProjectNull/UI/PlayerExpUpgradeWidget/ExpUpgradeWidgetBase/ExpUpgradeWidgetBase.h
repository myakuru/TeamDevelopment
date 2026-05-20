// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpUpgradeWidgetBase.generated.h"

class UTextBlock;
class UImage;
class UExpUpgradeWidget0;
struct FExpUpgradeRow;

/**
 * データーテーブルから取得したテキストをUIにするクラスのベースクラス
 */
UCLASS()
class PROJECTNULL_API UExpUpgradeWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void ChoicesExpUpgrade();

	// マウスがウィジェットに入ったとき
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// マウスがウィジェットを離れたとき
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/** マウスがウィジェット上にあるかどうかを取得 */
	bool IsMouseOver() const { return bIsMouseOver; }

	void ImageRotation();

	void InitExpUpgradeWidget();

	/** クリックで呼び出してレベルアップを行う */
	void OnUpgradeClicked();

protected:
	/** テキストを表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	/** 画像を表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> UpgradeImage;

	/** UIのスケールの変化速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleSpeed = { 0.0f,0.0f };

	/** UIの最大スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleMax = { 0.0f,0.0f };

	/** UIの最小スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleMin = { 0.0f,0.0f };

	/** 現在のUIのスケール */
	FVector2D UiScale = { 0.0f,0.0f };

private:
	/** 明示的な行名を渡してレベルアップする（クリック時にRow名を渡す場合はこちらを呼ぶ） */
	void OnUpgradeClickedByName(FName ClickedRowName);

	int32 GetRandomTextId();

	bool bIsMouseOver = false;

	/** プレイヤー側で保持する各行ごとの現在レベル（デフォルトは 0） */
	TMap<FName, int32> PlayerUpgradeLevels;

	/** DataTable をキャッシュしておく（毎回ロードしない） */
	UPROPERTY()
	UDataTable* CachedExpUpgradeTable = nullptr;

	/** 現在表示中の行名（ChoicesExpUpgrade でセット） */
	FName SelectedRowName = NAME_None;

	/** 現在表示中の行データへのポインタ（DataTable の行をキャッシュ） */
	const FExpUpgradeRow* SelectedRowData = nullptr;

	/** 指定した行に対して現在レベルのテキストを表示する */
	void ShowTextForCurrentLevel(FName RowName, const struct FExpUpgradeRow* RowData);

	/** DataTable のロード（内部でキャッシュ） */
	UDataTable* GetExpUpgradeTable();
};
