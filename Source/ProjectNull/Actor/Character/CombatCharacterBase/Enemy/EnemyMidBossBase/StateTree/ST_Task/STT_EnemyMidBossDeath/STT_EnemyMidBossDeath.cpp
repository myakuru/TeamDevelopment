#include "STT_EnemyMidBossDeath.h"
#include "StateTreeExecutionContext.h"

#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyMidBossBase\EnemyMidBossBase.h>

USTT_EnemyMidBossDeath::USTT_EnemyMidBossDeath(const FObjectInitializer& InObjInit)
	:	Super(InObjInit)
	,	OwnerEnemy(nullptr)
{
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyMidBossDeath::EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	OwnerEnemy = Cast<AEnemyMidBossBase>(InContext.GetOwner());
	if (!IsValid(OwnerEnemy)) { return EStateTreeRunStatus::Failed; }

	// 死亡に適したアニメーションを再生
	{
		// メッシュチェック
		USkeletalMeshComponent* Mesh = OwnerEnemy->GetMesh();
		UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
		if (!IsValid(Anim) || !IsValid(DeathAnim)) { return EStateTreeRunStatus::Failed; }

		// アニメーションの長さと遅延時間を合わせて取得
		DeathAnimTimer = Anim->Montage_Play(DeathAnim) + DeathDurationTime;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyMidBossDeath::Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)
{
	if (!IsValid(OwnerEnemy)) { return EStateTreeRunStatus::Failed; }

	// タイマーが時間切れになったら終了
	if (DeathAnimTimer < 0.f)
	{
		return EStateTreeRunStatus::Succeeded;
	}
	
	// タイマー更新
	DeathAnimTimer -= InDeltaTime;

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyMidBossDeath::ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	if (!IsValid(OwnerEnemy)) { return; }

	OwnerEnemy->FinalizeDeath();
}
