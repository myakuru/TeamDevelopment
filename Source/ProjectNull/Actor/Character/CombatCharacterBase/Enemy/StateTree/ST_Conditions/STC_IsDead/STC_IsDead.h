#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsDead.generated.h"

/**
 * 死亡しているかを調べる条件クラス
 */
UCLASS()
class PROJECTNULL_API USTC_IsDead : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()

protected:

	/**
	 * @brief 敵が死んでいるかを判定する処理
	 * @param Context 持ち主のコンテキスト
	 * @return 体力が一定以下ならtrue
	 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

private:

	UPROPERTY(visible,Category="Input")
	float CurrentHelth=0.0f;

};
