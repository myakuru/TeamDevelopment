#pragma once

//----------------------------------------------------------
// Includes
//----------------------------------------------------------
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageNumberWidget.generated.h"

//----------------------------------------------------------
// Forward Declaration
//----------------------------------------------------------
class UTextBlock;
class UWidgetAnimation;

//----------------------------------------------------------
// UDamageNumberWidget class
//----------------------------------------------------------
UCLASS()
class PROJECTNULL_API UDamageNumberWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// ダメージ値をUIに反映
	UFUNCTION(BlueprintCallable)
	void SetDamageValue(int32 Damage, bool bCritical = false);

	// 表示アニメーション再生
	UFUNCTION(BlueprintCallable)
	void PlayPopupAnimation();

protected:

	// WidgetBlueprint側のTextBlock名をDamageTextにする
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> DamageText = nullptr;

	// WidgetBlueprint側のAnimation名をPopupAnimにする
	UPROPERTY(Transient, meta = (BindWidgetAnimOptional))
	TObjectPtr<UWidgetAnimation> PopupAnim = nullptr;
};
