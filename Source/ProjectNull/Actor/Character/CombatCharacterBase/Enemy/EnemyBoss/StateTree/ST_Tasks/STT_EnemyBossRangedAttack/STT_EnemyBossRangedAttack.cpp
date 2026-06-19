#include "STT_EnemyBossRangedAttack.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossRangedAttack::USTT_EnemyBossRangedAttack(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossRangedAttack::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	// Tick：今のモンタージュが終わったら次へ
	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	if (!Atk.AttackMontages.IsValidIndex(Boss->GetHitIndex()))
	{
		return EStateTreeRunStatus::Succeeded;
	}
	UAnimMontage* Cur = Atk.AttackMontages[Boss->GetHitIndex()];
	if (!Anim->Montage_IsPlaying(Cur))
	{
		Boss->AdvanceHitIndex();
		if (Atk.AttackMontages.IsValidIndex(Boss->GetHitIndex()))
		{
			Anim->Montage_Play(Atk.AttackMontages[Boss->GetHitIndex()]);
			return EStateTreeRunStatus::Running;
		}
		return EStateTreeRunStatus::Succeeded;
	}
	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossRangedAttack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	// ボスを確認して、選択中の攻撃を受け取る
	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(OwnerBoss);
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// AnimInstanceを取得して再生
	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim))
	{
		return EStateTreeRunStatus::Failed;
	}

	// EnterState：最初の一撃を再生
	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	if (Atk.AttackMontages.Num() == 0) { return EStateTreeRunStatus::Failed; }

	Anim->Montage_Play(Atk.AttackMontages[0]);
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossRangedAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
