#include "EnemyRuntimeData.h"

UEnemyRuntimeData::UEnemyRuntimeData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
	,	StateEnum(EEnemyState::Idle)
	,	IsAlive(true)
{
}

void UEnemyRuntimeData::CalcDistanceToTarget(const FVector& a_TargetPos, const FVector& a_OwnerPos)
{
	// 移動方向・距離の二乗値を算出
	const FVector	NewMoveDir			= a_TargetPos - a_OwnerPos;
	const float		NewTargetDistSqr	= NewMoveDir.SizeSquared();
	
	// 進行方向に変化があればブロードキャスト発火
	if (!MoveDir.Equals(NewMoveDir))
	{
		MoveDir = NewMoveDir;
		MoveDir.Normalize();
		OnMoveDirChanged.Broadcast(MoveDir);
	}

	// 距離に変化があればブロードキャスト発火
	if (!FMath::IsNearlyEqual(TargetDistanceSqr, NewTargetDistSqr))
	{
		TargetDistanceSqr = NewTargetDistSqr;
		OnTargetDistChanged.Broadcast(TargetDistanceSqr);
	}
}

void UEnemyRuntimeData::ChangedEnemyState(EEnemyState a_StateEnum)
{
	if (StateEnum == a_StateEnum) { return; }

	// 変更があればブロードキャスト発火
	StateEnum = a_StateEnum;
	OnStateEnumChanged.Broadcast(StateEnum);
}

void UEnemyRuntimeData::ChangedIsAlive(const bool a_IsAlive)
{
	if (IsAlive == a_IsAlive) { return; }

	// 変更があればブロードキャスト発火
	IsAlive = a_IsAlive;
	OnIsAliveChanged.Broadcast(IsAlive);
}