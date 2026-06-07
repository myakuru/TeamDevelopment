#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossApproachWalk.generated.h"

class AEnemyBossBase;

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossApproachWalk : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossApproachWalk(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MoveSpeed = 200.0f;

};