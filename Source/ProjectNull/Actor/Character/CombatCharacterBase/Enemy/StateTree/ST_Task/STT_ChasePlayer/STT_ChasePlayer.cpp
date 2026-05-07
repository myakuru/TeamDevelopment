
#include "STT_ChasePlayer.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

EStateTreeRunStatus USTT_ChasePlayer::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_ChasePlayer::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Super::Tick(a_Context, a_DeltaTime);

	return EStateTreeRunStatus::Running;
}

void USTT_ChasePlayer::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}


void USTT_ChasePlayer::MoveToPlayer()
{
}
