#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GetGearHUDWidget.generated.h"

class UTextBlock;
class UImage;
class UButtonBaseWidget;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UGetGearHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetGearData(const FName& inGearName);

	UFUNCTION()
	void OpenUI();

private:

	virtual void NativeOnInitialized()override;

	virtual void NativeDestruct()override;

	UFUNCTION()
    void RemoveSelf();

	UFUNCTION()
	void OnFadeOutAnimFinished();

	/** UIパーツ */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GearName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GearImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButtonBaseWidget> CloseUIButton;

	/** アニメーション */
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeInAnim;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeOutAnim;
};
