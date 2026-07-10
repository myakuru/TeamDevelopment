#include "STC_EnemyBossIsDead.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>

bool USTC_EnemyBossIsDead::TestCondition(FStateTreeExecutionContext& Context) const
{
	const AEnemyBossBase* Boss = Cast<AEnemyBossBase>(OwnerBoss);
	if (!IsValid(Boss))
	{
		UE_LOG(LogTemp, Error, TEXT("STC_EnemyBossIsDead: Boss is invalid"));
		return false;
	}

	const bool bAlive = Boss->BossIsAlive();
	const bool bDead = !bAlive;

	UE_LOG(LogTemp, Warning, TEXT("STC_EnemyBossIsDead: bAlive=%s bDead=%s"),
		bAlive ? TEXT("true") : TEXT("false"),
		bDead ? TEXT("true") : TEXT("false"));

	return bDead;
}
