#include "EnemyBossRuntimeData.h"

UEnemyBossRuntimeData::UEnemyBossRuntimeData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
	,	IsAlive(true)
{
}

void UEnemyBossRuntimeData::ChangedIsAlive(const bool a_IsAlive)
{
	if (IsAlive == a_IsAlive) { return; }

	// 変更があればブロードキャスト発火
	IsAlive = a_IsAlive;
	OnIsAliveChanged.Broadcast(IsAlive);
}
