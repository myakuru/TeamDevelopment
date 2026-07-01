#include "EnemyRuntimeData.h"

UEnemyRuntimeData::UEnemyRuntimeData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
	,	StateEnum(EEnemyState::Idle)
	,	IsAlive(true)
{
}

void UEnemyRuntimeData::StartAnimMonitor(int32 a_AnimIndex, bool bLooping, float Duration)
{
	CPUAnim.AnimIndex	= a_AnimIndex;
	CPUAnim.bLooping	= bLooping;
	CPUAnim.bFinished	= false;
	CPUAnim.ElapsedTime = 0.0f;
	CPUAnim.Duration	= Duration;
}

void UEnemyRuntimeData::SetAttackFinishTime(float InLastTime)
{
	AttackFinishTime = InLastTime;
}

bool UEnemyRuntimeData::CanAttack()const 
{
	return GetWorld()->GetTimeSeconds() >= AttackInterval + AttackFinishTime;
}

void UEnemyRuntimeData::SetTargetLocation(const FVector& InTargetLocation)
{
	TargetLocation = InTargetLocation;
	OnTargetLocationChanged.Broadcast(TargetLocation);
}

void UEnemyRuntimeData::SetFinalHP(float InFinalHP)
{
	Health.Max = Health.Current = InFinalHP;
}

void UEnemyRuntimeData::SetBaseAttackPower(float InBasePower)
{
	Attack.Base = InBasePower;
}

void UEnemyRuntimeData::UpdateAnimationMonitor(float DeltaTime)
{
	if (CPUAnim.bLooping)	{ return; }
	if (CPUAnim.bFinished)	{ return; }

	CPUAnim.ElapsedTime += DeltaTime;

	if (CPUAnim.ElapsedTime >= CPUAnim.Duration)
	{
		CPUAnim.bFinished = true;
	}
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

void UEnemyRuntimeData::CalclateDamageToMaxHealthRatio(const float InReciveDamage)
{
	DamageToMaxHealthRatio = InReciveDamage / Health.Max;
	OnDamageRatioChanged.Broadcast(DamageToMaxHealthRatio);
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
