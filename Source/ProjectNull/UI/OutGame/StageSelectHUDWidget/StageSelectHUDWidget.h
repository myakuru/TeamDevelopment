#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectHUDWidget.generated.h"

class UStageDataAsset;
class UCanvasPanel;
class UTextBlock;
class UImage;
class UTexture2D;
class UStageButtonWidget;
class UButton;

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

	UFUNCTION()
	void OnClickedReturnButton();

	/** ステージ番号をクランプ */
	UFUNCTION(BlueprintCallable, Category = "UI")
	int32 ClampStageIndex(int32 InStageIndex);

	/** ステージ詳細用関数 */
	/*ポイントチェック(一応)*/
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	bool CheckStageDetailsExistence();

	/** ステージデータチェック */
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	bool CheckStageData(int32 CheckStageIndex);

	/** ステージ詳細更新(引数は配列番号) */
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	void ChangeStageDetails(int32 InStageIndex);

	/** 画像変更 */
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	void ChangeStageImageTexture(UTexture2D* NewImage);

	/** ハイスコア変更 */
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	void ChangeHighScoreText(int32 InStageIndex);

	/** 進捗変更 */
	UFUNCTION(BlueprintCallable, Category = "UI|Details")
	void ChangeProgressText(int32 InStageIndex);

	/** ステージデータの参照(ゲームインスタンスから取得) */
	UPROPERTY()
	TObjectPtr<UStageDataAsset> StageDataAsset;

	/** 現在選択されているステージのインデックス(0～) */
	UPROPERTY()
	int32 CurrentSelectedStageIndex = 0;

	/** ステージリストを表示するCanvasPanel */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> StageCanvas;

	/** ステージ詳細UI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StageImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> HighScoreText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Progress")
	FText HighScoreFrontText;

	/** 進捗 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> StageClearAchievementImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MidBossAppearsAchievementImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> MidBossWinAchievementImage;

	/** 点灯時 / 消灯時のテクスチャをエディタ側から設定できるようにする */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Progress")
	TObjectPtr<UTexture2D> MissionClearedTexture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI|Progress")
	TObjectPtr<UTexture2D> MissionNotClearedTexture;

	/** ステージボタン */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Button")
	TSubclassOf<UStageButtonWidget> StageButtonWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Button")
	FVector2D	StageButtonWidgetFirstPosition	= FVector2D(100, 100);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stage|Button")
	FVector2D	StageButtonWidgetInterval		= FVector2D(0, 100);

	/** 戻るボタン */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ReturnButton;
};
