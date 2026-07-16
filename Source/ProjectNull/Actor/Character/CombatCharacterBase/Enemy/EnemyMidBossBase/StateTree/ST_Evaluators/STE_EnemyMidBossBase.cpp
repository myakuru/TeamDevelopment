#include "STE_EnemyMidBossBase.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Data\CharacterRuntimeData\EnemyMidBossRuntimeData\EnemyMidBossRuntimeData.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyMidBossBase\EnemyMidBossBase.h>

void USTE_EnemyMidBossBase::TreeStart(FStateTreeExecutionContext& InContext)
{
	Super::TreeStart(InContext);

	OwnerEnemy = Cast<AEnemyMidBossBase>(InContext.GetOwner());
	if (!IsValid(OwnerEnemy)) { return; }

	// デリゲートへの関数登録
	RegisterDelegate();
}

void USTE_EnemyMidBossBase::Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)
{
	if (!IsValid(OwnerEnemy)) { return; }

	Super::Tick(InContext, InDeltaTime);
}

void USTE_EnemyMidBossBase::TreeStop(FStateTreeExecutionContext& InContext)
{
	Super::TreeStop(InContext);
}

void USTE_EnemyMidBossBase::RegisterDelegate()
{
	if (!IsValid(OwnerEnemy)) { return; }

	EnemyRuntimeData = OwnerEnemy->GetEnemyMidBossRuntimeData();
	if (!EnemyRuntimeData) { return; }

	// 移動方向
	EnemyRuntimeData->OnMoveDirChanged.AddUObject(this, &ThisClass::SetMoveDir);

	// ターゲットの座標
	EnemyRuntimeData->OnTargetLocationChanged.AddUObject(this, &ThisClass::SetTargetLocation);

	// 受けたダメージが最大体力に対してどれくらいの割合か
	EnemyRuntimeData->OnDamageRatioChanged.AddUObject(this, &ThisClass::SetDamageRatio);

	// 距離の二乗値
	EnemyRuntimeData->OnTargetDistChanged.AddUObject(this, &ThisClass::SetTargetDistanceSqr);

	// ステートEnumを切り替える
	EnemyRuntimeData->OnStateEnumChanged.AddUObject(this, &ThisClass::SetEnemyState);

	// 攻撃データを切り替える
	EnemyRuntimeData->OnAttackDataChanged.AddUObject(this, &ThisClass::SetAttackData);
}