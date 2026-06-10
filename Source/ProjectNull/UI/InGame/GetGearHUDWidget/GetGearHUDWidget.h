#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GetGearHUDWidget.generated.h"

class UTextBlock;
class UImage;
class UButton;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UGetGearHUDWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetGearData(const FText& inGearName);

	UFUNCTION()
	void OpenUI();

private:

	virtual void NativeOnInitialized()override;

	virtual void NativeDestruct()override;

	UFUNCTION()
    void RemoveSelf();

	/** UIパーツ */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GearName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GearImage;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> CloseUIButton;
};
