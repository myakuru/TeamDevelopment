#include "STE_EnemyBossBase.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>

void USTE_EnemyBossBase::TreeStart(FStateTreeExecutionContext& Context)
{
	Super::TreeStart(Context);

	// オーナー（このボス自身）を一度だけ確定する
	OwnerEnemyBoss = Cast<AEnemyBossBase>(Context.GetOwner());

	if (IsValid(OwnerEnemyBoss))
	{
		// AIControllerを取得しEvaluatorに登録
		AIC = OwnerEnemyBoss->GetController();
	}
}

void USTE_EnemyBossBase::TreeStop(FStateTreeExecutionContext& Context)
{
	Super::TreeStop(Context);
}

void USTE_EnemyBossBase::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	Super::Tick(Context, DeltaTime);

	if (!IsValid(OwnerEnemyBoss)) { return; }

	// ターゲットの更新はボス側コールバックが行う。ポインタを読むだけなので低コスト
	TargetActor = OwnerEnemyBoss->GetTargetActor();

	// AIControllerが遅れて生成される場合の保険
	if (!IsValid(AIC))
	{
		AIC = OwnerEnemyBoss->GetController();
	}
}
