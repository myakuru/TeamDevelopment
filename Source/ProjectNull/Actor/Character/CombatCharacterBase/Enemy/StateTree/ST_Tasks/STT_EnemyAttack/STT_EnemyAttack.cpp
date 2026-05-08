#include "STT_EnemyAttack.h"

USTT_EnemyAttack::USTT_EnemyAttack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyAttack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	return EStateTreeRunStatus();
}

EStateTreeRunStatus USTT_EnemyAttack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	return EStateTreeRunStatus();
}

void USTT_EnemyAttack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
}
