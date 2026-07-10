#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeConditionBlueprintBase.h"
#include "STC_EnemyBossIsDead.generated.h"

/**
 * 死亡しているかを調べる条件クラス
 */
UCLASS()
class PROJECTNULL_API USTC_EnemyBossIsDead : public UStateTreeConditionBlueprintBase
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

	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<ACharacter> OwnerBoss = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "Input")
	bool IsAlive = true;

};
