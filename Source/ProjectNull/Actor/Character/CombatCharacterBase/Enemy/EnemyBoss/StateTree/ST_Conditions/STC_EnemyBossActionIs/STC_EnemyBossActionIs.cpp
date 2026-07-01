#include "STC_EnemyBossActionIs.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>

bool USTC_ActionIs::TestCondition(FStateTreeExecutionContext& Context) const
{
	const AEnemyBossBase* Boss = Cast<AEnemyBossBase>(OwnerBoss);
	const bool bResult = IsValid(Boss) && Boss->GetCurrentAction() == TargetAction;

	UE_LOG(LogTemp, Warning, TEXT("ActionIs: Boss=%s Next=%d Target=%d -> %d"),
		*GetNameSafe(OwnerBoss),
		Boss ? (int32)Boss->GetCurrentAction() : -1,
		(int32)TargetAction,
		bResult ? 1 : 0);

	return bResult;
}
