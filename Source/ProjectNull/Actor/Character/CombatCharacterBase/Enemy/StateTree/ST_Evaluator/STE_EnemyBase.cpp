#include "STE_EnemyBase.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

void USTE_EnemyBase::TreeStart(FStateTreeExecutionContext& a_Context)
{
	Super::TreeStart(a_Context);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
}

void USTE_EnemyBase::TreeStop(FStateTreeExecutionContext& a_Context)
{
	Super::TreeStop(a_Context);
}

void USTE_EnemyBase::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return; }

	Super::Tick(a_Context, a_DeltaTime);
}
