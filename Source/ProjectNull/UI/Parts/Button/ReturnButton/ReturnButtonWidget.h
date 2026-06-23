#pragma once

#include "CoreMinimal.h"
#include "../ButtonBaseWidget.h"
#include "ReturnButtonWidget.generated.h"

/**
 *　共通のリターンボタン
 */
UCLASS()
class PROJECTNULL_API UReturnButtonWidget : public UButtonBaseWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION()
	FName GetOpenLevelName() const { return OpenLevelName; }

protected:

	UPROPERTY(EditAnywhere)
	FName OpenLevelName = "";
};
