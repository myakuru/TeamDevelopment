#include "STT_EnemyAttack.h"
#include "Kismet/GameplayStatics.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\System\Combat\Attack\AttackBase.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\Component\EnemyAttackComponent\EnemyAttackComponent.h>
#include <ProjectNull/Data/CharacterRuntimeData/EnemyRuntimeData/EnemyRuntimeData.h>

USTT_EnemyAttack::USTT_EnemyAttack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	OwnerEnemy(nullptr)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyAttack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);
	
	// オーナーセット
	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy)	{ return EStateTreeRunStatus::Failed; }

	if (auto AttackComponent = OwnerEnemy->GetEnemyAttackComponent())
	{
		AttackComponent->TestActive();
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyAttack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	if (auto EnemyRuntime = OwnerEnemy->GetEnemyRuntimeData())
	{
		// アニメが1周したらSucceededを返してStateTreeに遷移を委ねる
		if (EnemyRuntime->GetAnimFinished())
		{
			// ステートタイプを切り替え
			OwnerEnemy->NotifyChangedStateEnum(EEnemyState::Idle);

			return EStateTreeRunStatus::Succeeded;
		}
	}
	
	return EStateTreeRunStatus::Running;
}