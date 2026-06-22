#pragma once

#include "CoreMinimal.h"
#include "../../../Parts/Button/ButtonBaseWidget.h"
#include "Delegates/DelegateCombinations.h"
#include "StageButtonWidget.generated.h"

/** デリゲート宣言(子デリゲート) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedStageButton, int32, InStageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredStageButton, int32, InStageIndex);

/**
 * stageSelectHUDWidgetで使用するstageボタン
 */
UCLASS()
class PROJECTNULL_API UStageButtonWidget : public UButtonBaseWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct()override;

	virtual void DoHoveredButton() override;

	virtual void DoClickedButton() override;

	UFUNCTION()
	void DoHoveredStageButton();

	UFUNCTION()
	void DoClickedStageButton();

	UPROPERTY()
	int32	StageIndex	= 1;

	UPROPERTY()
	bool	bUnlocked	= false;

public:

	/** ボタンのセットアップ関数 */
	UFUNCTION()
	void Setup(int32 InStageIndex, bool bInUnlocked);

	//デリゲートの宣言
	UPROPERTY(BlueprintAssignable)
	FOnClickedStageButton OnClickedStage;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredStageButton OnHoveredStage;
};
