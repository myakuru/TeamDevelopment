#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyAttack.generated.h"

class UAttackBase;
class AEnemyBase;

/**
 * 敵の攻撃ステートタスク
 * どんな攻撃にするかはエディター側で編集
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyAttack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyAttack(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	/**
	 * @brief 攻撃判定処理
	 */
	void AttackJudge(const float a_DeltaTime);

protected:

	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy;

};
