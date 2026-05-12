#include "STT_EnemyDead.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h>

USTT_EnemyDead::USTT_EnemyDead(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyDead::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// 死亡アニメーションの再生やエフェクトの発生を行う
	OwnerEnemy->SetEnemyState(EEnemyState::Dead);

	return EStateTreeRunStatus();
}

EStateTreeRunStatus USTT_EnemyDead::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyDead::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}
