#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageButtonWidget.generated.h"

class UButton;

/**
 * stageSelectHUDWidgetで使用するstageボタン
 */
UCLASS()
class PROJECTNULL_API UStageButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> StageButton;

	UFUNCTION()
	void OnClickedStageButton();

	UPROPERTY()
	int32 StageIndex = 0;

public:

	/** ボタンのセットアップ関数 */
	UFUNCTION()
	void Setup(int32 InStageIndex);

};
