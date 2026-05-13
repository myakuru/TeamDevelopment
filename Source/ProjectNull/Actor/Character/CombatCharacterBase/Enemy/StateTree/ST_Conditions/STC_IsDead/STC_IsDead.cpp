#include "STC_IsDead.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

bool USTC_IsDead::TestCondition(FStateTreeExecutionContext& Context) const
{
	Super::TestCondition(Context);

	// 敵の体力が０以下ならtrue
	if (IsAlive)
	{
		return false;
	}
	else
	{
		return true;
	}
}
