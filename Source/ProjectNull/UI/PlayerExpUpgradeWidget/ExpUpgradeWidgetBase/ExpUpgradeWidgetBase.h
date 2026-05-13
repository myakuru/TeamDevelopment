// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpUpgradeWidgetBase.generated.h"

class UTextBlock;
class UImage;
class UExpUpgradeWidget0;

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

	// 左ボタン押下を処理する
	//virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// マウスがウィジェットに入ったとき
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	// マウスがウィジェットを離れたとき（必要なら）
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

protected:
	/** テキストを表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

	/** 画像を表示するためのウィジェット */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> UpgradeImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleSpeed = { 0.0f,0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScale = { 0.0f,0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	FVector2D UiScaleMax = { 0.0f,0.0f };
private:
	int32 GetRandomTextId();

	void ImageRotation();
};
