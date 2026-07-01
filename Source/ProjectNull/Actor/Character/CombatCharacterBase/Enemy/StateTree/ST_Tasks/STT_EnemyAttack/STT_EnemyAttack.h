#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyAttack.generated.h"

class UAttackBase;
class AEnemyBase;

/**
 * 敵の攻撃ステートタスク
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyAttack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyAttack(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;

protected:

	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy;

};
