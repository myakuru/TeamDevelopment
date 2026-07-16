#include "STT_EnemyMidBossWalk.h"
#include "StateTreeExecutionContext.h"
#include "GameFramework\CharacterMovementComponent.h"

#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyMidBossBase\EnemyMidBossBase.h>

USTT_EnemyMidBossWalk::USTT_EnemyMidBossWalk(const FObjectInitializer& InObjInit)
	:	Super(InObjInit)
{
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyMidBossWalk::EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	OwnerEnemy = Cast<AEnemyMidBossBase>(InContext.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// 回転速度を取得
	RotationInterpSpeed = OwnerEnemy->GetRotationInterpSpeed();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyMidBossWalk::Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Super::Tick(InContext, InDeltaTime);

	// 移動方向に回転
	RotationToMoveDir(InDeltaTime);

	// 移動処理
	Move(InDeltaTime);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyMidBossWalk::RotationToMoveDir(const float InDeltaTime)
{
	if (!OwnerEnemy) { return; }

	const FRotator WantRot(0.f, MoveDir.Rotation().Yaw, 0.f);	// 目標角度となる移動方向のYaw
	const FRotator NowRot = OwnerEnemy->GetActorRotation();		// 現在の回転
	
	// 回転補間
	OwnerEnemy->SetActorRotation(
		FMath::RInterpTo(NowRot, WantRot, InDeltaTime, RotationInterpSpeed)
	);
}

void USTT_EnemyMidBossWalk::Move(const float InDeltaTime)
{
	if (!OwnerEnemy) { return; }

	OwnerEnemy->AddMovementInput(MoveDir);
}
