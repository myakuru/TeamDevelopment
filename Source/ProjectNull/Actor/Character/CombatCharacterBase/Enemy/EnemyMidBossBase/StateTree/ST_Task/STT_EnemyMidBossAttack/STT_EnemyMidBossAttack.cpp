#include "STT_EnemyMidBossAttack.h"
#include "StateTreeExecutionContext.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull\Component\EnemyAttackComponent\EnemyAttackComponent.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyMidBossBase\EnemyMidBossBase.h>

USTT_EnemyMidBossAttack::USTT_EnemyMidBossAttack(const FObjectInitializer& InObjInit)
	:	Super(InObjInit)
	,	OwnerEnemy(nullptr)
	,	AttackData(FEnemyAttackData())
{
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyMidBossAttack::EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	OwnerEnemy = Cast<AEnemyMidBossBase>(InContext.GetOwner());
	if (!IsValid(OwnerEnemy)) { return EStateTreeRunStatus::Failed; }
	
	// プレイヤーの座標を取得
	FVector TargetLocation=FVector::ZeroVector;
	if (TObjectPtr<APawn> PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		TargetLocation = PPlayerPawn->GetActorLocation();
	}
		
	// 攻撃を実行
	OwnerEnemy->ActivateAttack(AttackData.AttackType,TargetLocation);
	
	// 攻撃に適したアニメーションを再生
	{
		// メッシュチェック
		USkeletalMeshComponent* Mesh = OwnerEnemy->GetMesh();
		UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
		if (!IsValid(Anim)||!IsValid(AttackData.Animation)) { return EStateTreeRunStatus::Failed; }
		Anim->Montage_Play(AttackData.Animation);
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyMidBossAttack::Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)
{
	Super::Tick(InContext, InDeltaTime);

	if (!IsValid(OwnerEnemy)) { return EStateTreeRunStatus::Failed; }

	// メッシュチェック
	USkeletalMeshComponent* Mesh = OwnerEnemy->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }
	
	// アニメーションが終了したらステート終了
	if (!Anim->Montage_IsPlaying(AttackData.Animation))
	{
		// ステートをリセット
		OwnerEnemy->NotifyChangedStateEnum(EEnemyState::Idle);
		return EStateTreeRunStatus::Succeeded;
	}

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyMidBossAttack::ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)
{
	Super::ExitState(InContext, InTransition);
	
	OwnerEnemy->FinalizeAttackEnd();
}
