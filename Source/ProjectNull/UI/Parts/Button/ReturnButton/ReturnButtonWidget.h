#pragma once

#include "CoreMinimal.h"
#include "../ButtonBaseWidget.h"
#include "ReturnButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UReturnButtonWidget : public UButtonBaseWidget
{
	GENERATED_BODY()
	
protected:

	virtual void DoClickedButton()override;

	UPROPERTY(EditAnywhere)
	FName OpenLevelName = "";
};
