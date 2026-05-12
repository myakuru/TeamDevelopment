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
