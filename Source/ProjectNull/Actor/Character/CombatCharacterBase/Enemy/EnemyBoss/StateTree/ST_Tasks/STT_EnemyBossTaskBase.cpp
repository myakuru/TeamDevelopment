#include "STT_EnemyBossTaskBase.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>

USTT_EnemyBossTaskBase::USTT_EnemyBossTaskBase(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	bShouldCallTick = true;   // ボスタスクは基本Tickを使うので共通で有効化
}

AEnemyBossBase* USTT_EnemyBossTaskBase::GetBoss() const
{
	return Cast<AEnemyBossBase>(OwnerBoss);
}

bool USTT_EnemyBossTaskBase::HasValidRefs() const
{
	return IsValid(OwnerBoss) && IsValid(TargetActor) && IsValid(AIC);
}
