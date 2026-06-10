#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GetGearHUDWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UGetGearHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:

	void NativeConstruct() override{}

	/** UIパーツ */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> GearName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> GearImage;
};
