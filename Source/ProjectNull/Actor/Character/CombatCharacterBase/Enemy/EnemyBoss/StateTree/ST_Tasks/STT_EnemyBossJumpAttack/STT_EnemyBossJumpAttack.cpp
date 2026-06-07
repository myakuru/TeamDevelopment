#include "STT_EnemyBossJumpAttack.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossJumpAttack::USTT_EnemyBossJumpAttack(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossJumpAttack::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("JumpAttack Tick In"));

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	if (!Atk.AttackMontages.IsValidIndex(0) || !Anim->Montage_IsPlaying(Atk.AttackMontages[0]))
	{
		return EStateTreeRunStatus::Succeeded;   // アニメ終了 → Decideへ
	}
	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossJumpAttack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss) || !IsValid(TargetActor)) { return EStateTreeRunStatus::Failed; }

	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	if (Atk.AttackMontages.Num() == 0) { return EStateTreeRunStatus::Failed; }

	// プレイヤー方向へ放物線で跳ぶ
	FVector Dir = TargetActor->GetActorLocation() - Boss->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();
	const FVector Launch = Dir * LaunchHorizontal + FVector(0, 0, LaunchVertical);
	Boss->LaunchCharacter(Launch, true, true);

	// アニメ再生
	if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
	{
		Anim->Montage_Play(Atk.AttackMontages[0]);
	}
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossJumpAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
