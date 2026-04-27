#include "STC_TestBlueprintBase.h"

bool USTC_TestBlueprintBase::TestCondition(FStateTreeExecutionContext& Context) const
{
	if (FrameTimer > 120)
	{
		UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!"));
		return true;
	}

	return false;
}
