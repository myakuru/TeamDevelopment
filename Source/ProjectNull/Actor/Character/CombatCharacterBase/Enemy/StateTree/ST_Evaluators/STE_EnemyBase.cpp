#include "STE_EnemyBase.h"
#include <Kismet/GameplayStatics.h>
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>

void USTE_EnemyBase::TreeStart(FStateTreeExecutionContext& a_Context)
{
	Super::TreeStart(a_Context);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	// デリゲートへの関数登録
	RegisterDelegate();

	// 固定パラメーターの登録
	RegisterFixedParams();
}

void USTE_EnemyBase::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return; }

	Super::Tick(a_Context, a_DeltaTime);
}

void USTE_EnemyBase::TreeStop(FStateTreeExecutionContext& a_Context)
{
	Super::TreeStop(a_Context);
}

void USTE_EnemyBase::RegisterFixedParams()
{
	if (!OwnerEnemy) { return; }

	// ノックバックの時の重さ
	KnockBackWeight = OwnerEnemy->GetKnockBackWeight();
	
	// 移動速度
	MoveSpeed = OwnerEnemy->GetMoveSpeed();

	// 回転補間速度
	RotateInterpSpeed = OwnerEnemy->GetRotationInterpSpeed();

	// 歩くことのできる斜面の角度
	WalkableFloorAngle = OwnerEnemy->GetWalkableFloorAngle();

	// 超える事の出来る段差の最大の高さ
	MaxStepHeight = OwnerEnemy->GetMaxStepHeight();

	// カプセルの半径分の高さ
	CapsuleHalfHeight = OwnerEnemy->GetCapsuleHalfHeight();
}

void USTE_EnemyBase::RegisterDelegate()
{
	if (!OwnerEnemy) { return; }

	EnemyRuntimeData = OwnerEnemy->GetEnemyRuntimeData();
	if (!EnemyRuntimeData) { return; }

	// ~ AddUObject() ~
	// GC管理・Weak参照
	// EnemyDestroyした後、残存しても呼ばれない

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

	// 死んでいるかどうか
	EnemyRuntimeData->OnIsAliveChanged.AddUObject(this, &ThisClass::SetIsAlive);
}