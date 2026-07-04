#include "STT_EnemyMidBossAttackSelect.h"
#include "StateTreeExecutionContext.h"

#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyMidBossBase\EnemyMidBossBase.h>

USTT_EnemyMidBossAttackSelect::USTT_EnemyMidBossAttackSelect(const FObjectInitializer& InObjInit)
	:	Super(InObjInit)
{
}

EStateTreeRunStatus USTT_EnemyMidBossAttackSelect::EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	OwnerEnemy = Cast<AEnemyMidBossBase>(InContext.GetOwner());
	if(!IsValid(OwnerEnemy)){ return EStateTreeRunStatus::Failed; }

	// 最小距離(負の数から)とその攻撃データ
	float MinDistanceSqr = -1.f;
	FEnemyAttackData MinAttackData;

	// どの攻撃範囲内に標的がいるか
	for (const auto& Attack : AttackDataAsset->AttackDatas)
	{
		const float MaxDistanceSqr = FMath::Square(Attack.MaxDistance);
		if (TargetDistanceSqr > MaxDistanceSqr)
		{
			// 最初の比較なら条件無しに代入
			if (MinDistanceSqr < 0.f)
			{
				MinDistanceSqr = MaxDistanceSqr;
				MinAttackData = Attack;
			}
			// 最小距離よりも短ければそれを採用
			else if (MinDistanceSqr < MaxDistanceSqr)
			{
				MinDistanceSqr = MaxDistanceSqr;
				MinAttackData = Attack;
			}
		}
	}

	// もし、攻撃クラスが入っていないなら中止
	if (!IsValid(MinAttackData.AttackClass)) { return EStateTreeRunStatus::Failed; }
	// 発動する攻撃データを選択
	OwnerEnemy->NotifyChangedAttackData(MinAttackData);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyMidBossAttackSelect::ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	Super::ExitState(InContext, InTransition);


}
