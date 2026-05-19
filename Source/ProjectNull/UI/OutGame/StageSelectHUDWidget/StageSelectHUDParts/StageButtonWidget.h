#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Delegates/DelegateCombinations.h"
#include "StageButtonWidget.generated.h"

class UButton;

/** デリゲート宣言 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnClickedStageButton, int32, InStageIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHoveredStageButton, int32, InStageIndex);

/**
 * stageSelectHUDWidgetで使用するstageボタン
 */
UCLASS()
class PROJECTNULL_API UStageButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StageButton;

	UFUNCTION()
	void OnClickedStageButton();

	UFUNCTION()
	void OnHoveredStageButton();

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
	FOnClickedStageButton OnClicked;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredStageButton OnHovered;
};
