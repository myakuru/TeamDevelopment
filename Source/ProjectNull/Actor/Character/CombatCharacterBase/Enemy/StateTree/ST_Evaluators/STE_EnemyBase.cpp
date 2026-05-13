#include "STE_EnemyBase.h"
#include <Kismet/GameplayStatics.h>
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>

void USTE_EnemyBase::TreeStart(FStateTreeExecutionContext& a_Context)
{
	Super::TreeStart(a_Context);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	UE_LOG(LogTemp, Warning, TEXT("GameGravity : %.2f"), GetWorld()->GetGravityZ());

	// デリゲートへの関数登録
	RegisterDelegate();
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

void USTE_EnemyBase::RegisterDelegate()
{
	if (!OwnerEnemy) { return; }

	EnemyRuntimeData = OwnerEnemy->GetEnemyRuntimeData();
	if (!EnemyRuntimeData) { return; }

	// ~ AddUObject() ~
	// GC管理・Weak参照
	// EnemyDestroyした後、残存しても呼ばれない

	// 移動方向
	EnemyRuntimeData->OnMoveDirChanged.AddUObject(this, &USTE_EnemyBase::SetMoveDir);

	// 距離の二乗値
	EnemyRuntimeData->OnTargetDistChanged.AddUObject(this, &USTE_EnemyBase::SetTargetDistanceSqr);

	// ステートEnumを切り替える
	EnemyRuntimeData->OnStateEnumChanged.AddUObject(this, &USTE_EnemyBase::SetEnemyState);

	// 死んでいるかどうか
	EnemyRuntimeData->OnIsAliveChanged.AddUObject(this, &USTE_EnemyBase::SetIsAlive);
}