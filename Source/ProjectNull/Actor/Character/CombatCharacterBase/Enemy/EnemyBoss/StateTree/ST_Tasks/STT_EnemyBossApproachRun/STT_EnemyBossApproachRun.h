#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossApproachRun.generated.h"

class AEnemyBossBase;

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossApproachRun : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossApproachRun(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MoveSpeed = 400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float JumpChangeDistance = 1000.0f;	/** ジャンプ攻撃に移行する距離*/

	bool bJumpFlg = false;	/** 走り中にジャンプ攻撃に移行するかのフラグ*/

private:

	// ------------------------------------------------------------------------------------
	// private method
	// ------------------------------------------------------------------------------------
};