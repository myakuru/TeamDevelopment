#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_HasTarget.generated.h"

/*
* 追尾対象が存在するかを判定
* 視認/被弾の検知はボス側コールバックが行い、その結果をみるだけ
*/
UCLASS()
class PROJECTNULL_API USTC_HasTarget : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:

	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

public:

	/** EvaluatorのOutputをTargetActorにバインドする*/
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<AActor> TargetActor = nullptr;
	
};
