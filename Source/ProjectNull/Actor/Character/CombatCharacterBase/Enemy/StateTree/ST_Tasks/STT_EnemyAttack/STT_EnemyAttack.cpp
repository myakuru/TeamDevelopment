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
	,	TargetActor(nullptr)
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

	// 前ステートの終了フラグをリセット
	OwnerEnemy->GetEnemyRuntimeData()->ResetAnimFinished();
	// 再生したいアニメを設定（インデックス・ループOFF・ブレンド開始）
	OwnerEnemy->GetEnemyRuntimeData()->SetNextAnimData(static_cast<uint32>(EEnemyState::Attack), true, true);

	OwnerEnemy->PlayAnimation(static_cast<uint32>(EEnemyState::Attack), true);

	if (auto AttackComponent = OwnerEnemy->GetEnemyAttackComponent())
	{
		AttackComponent->TestActive();
	}

	// ターゲット(プレイヤー)をセット
	TargetActor = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!TargetActor)	{ return EStateTreeRunStatus::Failed; }

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyAttack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy|| !TargetActor) { return EStateTreeRunStatus::Failed; }

	if (auto AttackComponent = OwnerEnemy->GetEnemyAttackComponent())
	{
		// 攻撃全てが終了しているか
		if (AttackComponent->IsAllAttackDeactivate())
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}

	// アニメが1周したらSucceededを返してStateTreeに遷移を委ねる
	/*if (OwnerEnemy->GetEnemyRuntimeData()->GetAnimFinished())
	{
		return EStateTreeRunStatus::Succeeded;
	}*/
	
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyAttack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);

	// ステートタイプを切り替え
	OwnerEnemy->NotifyChangedStateEnum(EEnemyState::Idle);
}