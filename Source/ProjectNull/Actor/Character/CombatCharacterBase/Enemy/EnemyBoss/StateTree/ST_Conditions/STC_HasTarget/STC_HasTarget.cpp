#include "STC_HasTarget.h"

bool USTC_HasTarget::TestCondition(FStateTreeExecutionContext& Context) const
{
	return IsValid(TargetActor);
}
