#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_IsInAttackRange.generated.h"

/**
 * 攻撃可能範囲内かを調べる条件クラス
 */
UCLASS()
class PROJECTNULL_API USTC_IsInAttackRange : public UStateTreeConditionBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTC_IsInAttackRange(const FObjectInitializer& ObjectInitializer);

protected:

	/**
	 * @brief 攻撃可能距離内かを判定する処理
	 * @param Context 持ち主のコンテキスト
	 * @return 攻撃可能範囲内ならtrue
	 */
	virtual bool TestCondition(FStateTreeExecutionContext& Context) const override;

private:

	/**
	 * @brief ターゲットとの距離の二乗値
	 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	float TargetDistSqr = 0.0f;

	/**
	 * @brief 攻撃可能距離
	 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	float AttackRange = 0.0f;
	
};