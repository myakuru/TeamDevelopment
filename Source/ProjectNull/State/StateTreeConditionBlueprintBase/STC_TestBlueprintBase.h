#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_TestBlueprintBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API USTC_TestBlueprintBase : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:

	virtual bool TestCondition(FStateTreeExecutionContext& Context)const override;

public:

	UPROPERTY(EditAnywhere,Category=Parameter)
	int FrameTimer = 0.0f;
};
