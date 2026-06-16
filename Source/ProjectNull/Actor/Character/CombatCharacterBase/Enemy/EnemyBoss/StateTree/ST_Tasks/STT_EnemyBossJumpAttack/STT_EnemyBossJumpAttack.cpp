#include "STT_EnemyBossJumpAttack.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
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
	UCharacterMovementComponent* Move = Boss->GetCharacterMovement();

	switch (Phase)
	{
	case EBossJumpPhase::Takeoff:
		// 踏み切りmontageが終わったら、ターゲットへ向けて射出 → 落下攻撃montageへ
		if (!Anim->Montage_IsPlaying(Atk.AttackMontages[0]))
		{
			FVector OutVel;
			const FVector Start = Boss->GetActorLocation();
			const FVector End = TargetActor->GetActorLocation();
			if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(Boss, OutVel, Start, End, 0.f, ArcParam))
			{
				Boss->LaunchCharacter(OutVel, true, true);
			}
			else
			{
				// アーク解が無い（近すぎる等）場合は素直に上+前へ跳ばす
				FVector Dir = (End - Start); Dir.Z = 0.f; Dir.Normalize();
				Boss->LaunchCharacter(Dir * 600.f + FVector(0, 0, 700.f), true, true);
			}

			Anim->Montage_Play(Atk.AttackMontages[1]);  // 落下〜着地攻撃
			bLandMontagePlayed = true;
			Phase = EBossJumpPhase::Air;
		}
		return EStateTreeRunStatus::Running;

	case EBossJumpPhase::Air:
		if (!Move->IsMovingOnGround()) { bLeftGround = true; }   // 離陸を確認

		// 落下攻撃montageが流れている間は待つ。
		// 「一度離陸して着地した」or「montageが再生終了した」で終わる
		{
			const bool bLanded = bLeftGround && Move->IsMovingOnGround();
			const bool bAnimDone = !Anim->Montage_IsPlaying(Atk.AttackMontages[1]);
			if (bLanded || bAnimDone)
			{
				Boss->SetActionPriority(EBossActionType::None);
				return EStateTreeRunStatus::Succeeded;
			}
		}
		return EStateTreeRunStatus::Running;
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
	if (!Atk.AttackMontages.IsValidIndex(1)) { return EStateTreeRunStatus::Failed; } // 2つ必須

	AIC->StopMovement();
	Phase = EBossJumpPhase::Takeoff;
	bLeftGround = false;
	bLandMontagePlayed = false;

	// アニメ再生
	if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
	{
		// 踏み切りmontage再生
		Anim->Montage_Play(Atk.AttackMontages[0]);
	}
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossJumpAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
