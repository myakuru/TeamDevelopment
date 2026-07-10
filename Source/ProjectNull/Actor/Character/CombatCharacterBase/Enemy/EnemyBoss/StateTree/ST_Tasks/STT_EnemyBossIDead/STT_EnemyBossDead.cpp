#include "STT_EnemyBossDead.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "GameFramework/Actor.h"

USTT_EnemyBossDead::USTT_EnemyBossDead(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossDead::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	if (IsValid(AIC))
	{
		AIC->StopMovement();
	}

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	Anim->Montage_Play(DeathMontage);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyBossDead::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	// モンタージュが終わったら終了
	if (!Anim->Montage_IsPlaying(DeathMontage))
	{
		return EStateTreeRunStatus::Succeeded;
	}


	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossDead::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return; }

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return; }

	Boss->BossFinalize();
	
}

