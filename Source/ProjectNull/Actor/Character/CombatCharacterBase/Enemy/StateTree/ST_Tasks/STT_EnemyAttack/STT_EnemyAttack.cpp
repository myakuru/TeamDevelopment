#include "STT_EnemyAttack.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\System\Combat\Attack\AttackBase.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

USTT_EnemyAttack::USTT_EnemyAttack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	OwnerEnemy(nullptr)
	,	Attack(nullptr)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyAttack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	if(!Attack){ return EStateTreeRunStatus::Failed; }
	
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Attack->Initialize(OwnerEnemy);

	TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!TargetActor) { return EStateTreeRunStatus::Failed; }

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyAttack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy || !Attack || !TargetActor) { return EStateTreeRunStatus::Failed; }

	Attack->Update(a_DeltaTime,TargetActor);

	Attack->AttackJudge(TargetActor);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyAttack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}
