#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectHUDWidget.generated.h"

class UTextBlock;
class UStageDataAsset;
class UVerticalBox;
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> StageNameText;

	/** ステージのデータリスト */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UStageDataAsset> StageDataAsset;

	/** ステージリストを表示するVerticalBox */
	//UPROPERTY(meta = (BindWidget))
	//UVerticalBox* StageListBox;

	UPROPERTY()
	int32 CurrentSelectedStageIndex = 0;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UStageButtonWidget> StageButton;
};
