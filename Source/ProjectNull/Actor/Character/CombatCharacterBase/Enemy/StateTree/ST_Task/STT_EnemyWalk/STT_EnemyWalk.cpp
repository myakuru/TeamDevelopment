#include "STT_EnemyWalk.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

EStateTreeRunStatus USTT_EnemyWalk::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyWalk::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Super::Tick(a_Context, a_DeltaTime);

	Move(a_DeltaTime);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyWalk::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}


void USTT_EnemyWalk::Move(const float a_DeltaTime)
{
	if (!OwnerEnemy) { return; }

	static float _RotSpeed = 5.0f;
	static float _MoveSpeed = 300.0f;

	// 回転角度補間
	const FRotator calcResultRot = CalculateRotationToMoveDir(
									OwnerEnemy->GetActorRotation(),
									MoveDir.Rotation(),
									_RotSpeed,
									a_DeltaTime
								);
	// 角度更新
	OwnerEnemy->SetActorRotation(calcResultRot);

	// 座標更新
	OwnerEnemy->SetActorLocation(
				CalculateNextActorLocation(
					OwnerEnemy->GetActorLocation(),
					MoveDir,
					_MoveSpeed,
					a_DeltaTime
				),
				true);
}

FVector USTT_EnemyWalk::CalculateNextActorLocation(const FVector& a_CurrentLocation, const FVector& a_MoveDir, float a_MoveSpeed, float a_DeltaTime)
{
	return a_CurrentLocation + a_MoveDir * a_MoveSpeed * a_DeltaTime;
}

FRotator USTT_EnemyWalk::CalculateRotationToMoveDir(const FRotator& a_CurrentRot, const FRotator& a_TargetRot, float a_RotSpeed, float a_DeltaTime)
{
	return FMath::RInterpTo(
						a_CurrentRot,
						a_TargetRot,
						a_RotSpeed,
						a_DeltaTime);
}
