#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossJumpAttack.generated.h"

class AEnemyBossBase;

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossJumpAttack : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossJumpAttack(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	/** 跳躍の水平速度 */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LaunchHorizontal = 900.0f;

	/** 跳躍の垂直速度 */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float LaunchVertical = 600.0f;

};