#include "STC_IsInAttackRange.h"

USTC_IsInAttackRange::USTC_IsInAttackRange(const FObjectInitializer& ObjectInitializer)
	:	Super(ObjectInitializer)
	,	TargetDistSqr(0.0f)
	,	AttackRange(0.0f)
{
}

bool USTC_IsInAttackRange::TestCondition(FStateTreeExecutionContext& Context) const
{
	// 攻撃範囲内ならtrue
	if (TargetDistSqr < FMath::Square(AttackRange))
	{
		return true;
	}
	else
	{
		return false;
	}
}