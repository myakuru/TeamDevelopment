#include "EnemyMidBossRuntimeData.h"

UEnemyMidBossRuntimeData::UEnemyMidBossRuntimeData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
	,	StateEnum(EEnemyState::Idle)
{
}

void UEnemyMidBossRuntimeData::NotifyAttackFinishTime()
{
	// 現在の時間を攻撃終了時間として登録
	AttackFinishTime = GetWorld()->GetTimeSeconds();
}

void UEnemyMidBossRuntimeData::SetTargetLocation(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
	OnTargetLocationChanged.Broadcast(TargetLocation);
}

bool UEnemyMidBossRuntimeData::HasAttackIntervalElapsed(float InInterval)
{
	return GetWorld()->GetTimeSeconds() > AttackFinishTime + InInterval;
}

void UEnemyMidBossRuntimeData::CalcDistanceToTarget(const FVector& InTargetPos, const FVector& InOwnerPos)
{
	// 移動方向・距離の二乗値を算出
	const FVector	NewMoveDir = InTargetPos - InOwnerPos;
	const float		NewTargetDistSqr = NewMoveDir.SizeSquared();

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

void UEnemyMidBossRuntimeData::CalclateDamageToMaxHealthRatio(const float InReciveDamage)
{
	DamageToMaxHealthRatio = InReciveDamage / Health.Max;
	OnDamageRatioChanged.Broadcast(DamageToMaxHealthRatio);
}

void UEnemyMidBossRuntimeData::ChangedEnemyState(EEnemyState InStateEnum)
{
	if (StateEnum == InStateEnum) { return; }

	// 変更があればブロードキャスト発火
	StateEnum = InStateEnum;
	OnStateEnumChanged.Broadcast(StateEnum);
}

void UEnemyMidBossRuntimeData::ChangedAttackData(const FEnemyAttackData& InAttackData)
{
	if (AttackData == InAttackData) { return; }

	// 変更があればブロードキャスト発火
	AttackData = InAttackData;
	OnAttackDataChanged.Broadcast(AttackData);
}
