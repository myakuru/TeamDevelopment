// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpUpgradeWidgetBase.generated.h"

class UTextBlock;
class UImage;
class UExpUpgradeWidget0;
class UPlayerRuntimeData;
struct FExpUpgradeRow;


USTRUCT(BlueprintType)
struct FUIParameter
{
	GENERATED_BODY()
	
	/** 通常時（非ホバー）の目標スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleMax = { 0.0f,0.0f };

	/** 出現アニメーションの開始スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleMin = { 0.0f,0.0f };

	/** ホバー時の目標スケール */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleHover = { 0.0f,0.0f };

	/** 現在のUIのスケール */
	FVector2D UiScale = { 0.0f,0.0f };
};

/**
 * データーテーブルから取得したテキストをUIにするクラスのベースクラス
 */
UCLASS()
class PROJECTNULL_API UExpUpgradeWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// マウスがウィジェットに入ったとき
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// マウスがウィジェットを離れたとき
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	/** マウスがウィジェット上にあるかどうかを取得 */
	bool IsMouseOver() const { return bIsMouseOver; }

	/** 毎フレーム呼び出し、現在のマウス状態に応じて目標スケールへ補間する */
	void UpdateScale();

	void InitExpUpgradeWidget();

	// 説明文をセットする
	void SetDescriptionText(const FText& Description);

	void SetUpgradeRowName(FName RowName) { UpgradeRowName = RowName; }

	FName GetUpgradeRowName() const { return UpgradeRowName; }

protected:
	/** テキストを表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	/** 画像を表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> UpgradeImage;

	/** スケール補間の速さ（大きいほど速く目標へ近づく） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	float ScaleInterpSpeed = 10.0f;

	// UIの画像のスケール値の変更
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "UIの画像のスケール値")
	FUIParameter UIImageParameter;
	
	// UIのテキストのスケール値の変更
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "UIのテキストのスケール値")
	FUIParameter UITextParameter;

private:

	bool bIsMouseOver = false;

	/** プレイヤー側で保持する各行ごとの現在レベル（デフォルトは 0） */
	TMap<FName, int32> PlayerUpgradeLevels;

	/** すでに表示した行を記録するセット（同じ行を複数回表示しないようにするため） */
	TSet<FName> ShownUpgradeRows;

	/** DataTable をキャッシュしておく（毎回ロードしない） */
	UPROPERTY()
	UDataTable* CachedExpUpgradeTable = nullptr;

	/** 現在表示中の行名（ChoicesExpUpgrade でセット） */
	FName SelectedRowName = NAME_None;

	/** 現在表示中の行データへのポインタ（DataTable の行をキャッシュ） */
	const FExpUpgradeRow* SelectedRowData = nullptr;

	/** DataTable のロード（内部でキャッシュ） */
	UDataTable* GetExpUpgradeTable();

	FName UpgradeRowName = NAME_None;
};
