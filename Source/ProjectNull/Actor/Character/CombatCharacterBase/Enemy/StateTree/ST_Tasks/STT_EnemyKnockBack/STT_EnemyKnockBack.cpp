#include "STT_EnemyKnockBack.h"

USTT_EnemyKnockBack::USTT_EnemyKnockBack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyKnockBack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	/* 備忘録 */
	// ・KnockBackFlagが有効の時に入る
	// ・デリゲートを用いて、フラグを管理する

	return EStateTreeRunStatus();
}

EStateTreeRunStatus USTT_EnemyKnockBack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	return EStateTreeRunStatus();
}

void USTT_EnemyKnockBack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
}
