#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectHUDWidget.generated.h"

class UTextBlock;
class UStageDataAsset;
class UCanvasPanel;
class UStageButtonWidget;

/**
 * ステージセレクト画面のHUDWidget
 */
UCLASS()
class PROJECTNULL_API UStageSelectHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

	void CreateStageButtons();

	/** ステージボタンのデリゲート */
	UFUNCTION()
	void OnClickedStageButton(int32 InStageIndex);

	UFUNCTION()
	void OnHoveredStageButton(int32 InStageIndex);

	/** ステージデータの参照 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	UStageDataAsset* StageDataList;

	/** 現在選択されているステージのインデックス */
	UPROPERTY()
	int32 CurrentSelectedStageIndex = 0;

	/** ステージ詳細UI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	/** ステージリストを表示するCanvasPanel */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> StageCanvas;

	/** ステージボタンのクラス参照 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage")
	TSubclassOf<UStageButtonWidget> StageButtonWidgetClass;
};
