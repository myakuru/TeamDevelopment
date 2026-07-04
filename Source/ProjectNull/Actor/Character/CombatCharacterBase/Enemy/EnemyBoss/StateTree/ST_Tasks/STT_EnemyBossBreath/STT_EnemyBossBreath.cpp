#include "STT_EnemyBossBreath.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossBreath::USTT_EnemyBossBreath(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossBreath::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	//UE_LOG(LogTemp, Warning, TEXT("Breath Tick In"));

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	UCharacterMovementComponent* Move = Boss->GetCharacterMovement();

	// まず向き直る、プレイヤーに向きなおったらMontage再生
	if (!bMontageStarted)
	{
		FVector Dir = TargetActor->GetActorLocation() - Boss->GetActorLocation();
		Dir.Z = 0.0f;
		Dir.Normalize();

		const FRotator Want(0.0f, Dir.Rotation().Yaw, 0.0f);
		const FRotator Now = Boss->GetActorRotation();
		Boss->SetActorRotation(FMath::RInterpTo(Now, Want, DeltaTime, 10.0f));

		// 十分プレイヤーを向けたら攻撃開始
		const float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(Now.Yaw, Want.Yaw));
		if (YawDiff <= 10.0f)   // 10度以内まで向いたらOK
		{
			//const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
			if (Atk.AttackMontages.Num() == 0) { return EStateTreeRunStatus::Failed; }
			//Anim->Montage_Play(Atk.AttackMontages[Boss->GetHitIndex()]);
			bMontageStarted = true;

			// 挙動を停止させてからアニメーション再生
			AIC->StopMovement();
			Phase = EBossBreathPhase::Start;
			// アニメ再生
			//if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				// montage再生
				Anim->Montage_Play(Atk.AttackMontages[0]);
			}
		}
		return EStateTreeRunStatus::Running;   // 向き直り中
	}

	switch (Phase)
	{
	case EBossBreathPhase::Start:
		if (!Anim->Montage_IsPlaying(Atk.AttackMontages[0]))
		{
			Phase = EBossBreathPhase::Loop;
			Anim->Montage_Play(Atk.AttackMontages[1]);

			Anim->Montage_JumpToSection(
				BreathLoopSectionName,
				Atk.AttackMontages[1]);

			Anim->Montage_SetNextSection(
				BreathLoopSectionName,
				BreathLoopSectionName,
				Atk.AttackMontages[1]);
		}
		return EStateTreeRunStatus::Running;
		break;
		case
		EBossBreathPhase::Loop:
			// ループ時間をカウント
			BreathCount += DeltaTime;
			if (BreathCount >= BreathDuration)
			{
				Phase = EBossBreathPhase::End;
				BreathCount = 0.0f;
				Anim->Montage_Stop(BreathEndBlend, Atk.AttackMontages[1]);
			}
			break;
		case EBossBreathPhase::End:
			// アニメーションブレンド
		{
			BreathCount += DeltaTime;
			if (BreathCount >= BreathEndBlend)
			{
				Boss->SetActionPriority(EBossActionType::None);
				AIC->StopMovement();
				Boss->SetPrevAction(Boss->GetCurrentAction());
				return EStateTreeRunStatus::Succeeded;
			}
		}
			return EStateTreeRunStatus::Running;
	}

	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossBreath::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss) || !IsValid(TargetActor)) { return EStateTreeRunStatus::Failed; }

	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	if (!Atk.AttackMontages.IsValidIndex(1)) { return EStateTreeRunStatus::Failed; } // 2つ必須

	if (IsValid(AIC))
	{
		AIC->StopMovement();
	}

	BreathCount = 0.0f;

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossBreath::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss))
	{
		return;
	}
}
