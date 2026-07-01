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
	//UE_LOG(LogTemp, Warning, TEXT("JumpAttack Tick In"));

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
			Phase = EBossJumpPhase::Takeoff;
			bLeftGround = false;
			bLandMontagePlayed = false;

			// アニメ再生
			//if (UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr)
			{
				// 踏み切りmontage再生
				Anim->Montage_Play(Atk.AttackMontages[0]);
				FVector OutVel;
				const FVector Start = Boss->GetActorLocation();
				const FVector End = TargetActor->GetActorLocation();
				if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(Boss, OutVel, Start, End, 0.0f, ArcParam))
				{
					Boss->LaunchCharacter(OutVel, true, true);
				}
				else
				{
					Boss->LaunchCharacter({ 0,0,600 }, true, true);
				}
			}
		}
		return EStateTreeRunStatus::Running;   // 向き直り中
	}

	switch (Phase)
	{
	case EBossJumpPhase::Takeoff:
		// 踏み切りmontageが終わったら、ターゲットへ向けて射出 → 落下攻撃montageへ
		if (!Anim->Montage_IsPlaying(Atk.AttackMontages[0]))
		{
			Phase = EBossJumpPhase::WaitLiftOff;
		}
		return EStateTreeRunStatus::Running;
		case
			EBossJumpPhase::WaitLiftOff:
				/*UE_LOG(LogTemp, Warning, TEXT("GravityScale = %f"), Move->GravityScale);
				UE_LOG(LogTemp, Warning, TEXT("JumpZVelocity = %f"), Move->JumpZVelocity);
				UE_LOG(LogTemp, Warning, TEXT("MovementMode = %d"), (int32)Move->MovementMode);
				UE_LOG(LogTemp, Warning, TEXT("IsMovingOnGround = %s, Velocity.Z = %f"),
					Move->IsMovingOnGround() ? TEXT("true") : TEXT("false"),
					Boss->GetVelocity().Z);*/
				//if (!Move->IsMovingOnGround())
				{
					// 離陸を確認したら落下攻撃Montage再生
					bLeftGround = true;
					Anim->Montage_Play(Atk.AttackMontages[1]); // 落下～着地攻撃
					Phase = EBossJumpPhase::Air;
				}
				break;
	case EBossJumpPhase::Air:
		// 落下攻撃montageが流れている間は待つ。
		// 「一度離陸して着地した」or「montageが再生終了した」で終わる
		{
			const bool bLanded = bLeftGround && Move->IsMovingOnGround();
			const bool bAnimDone = !Anim->Montage_IsPlaying(Atk.AttackMontages[1]);
			if (bLanded && bAnimDone)
			{
				Boss->SetActionPriority(EBossActionType::None);
				AIC->StopMovement();
				return EStateTreeRunStatus::Succeeded;
			}
		}
		return EStateTreeRunStatus::Running;
		break;
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

	Boss->RequestFastFallOnNotify();   // 落下時に重力加速するようにフラグを立てる
	Phase = EBossJumpPhase::Takeoff;
	bMontageStarted = false;

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossJumpAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) 
	{ return; }

	Boss->SetPrevAction(Boss->GetCurrentAction());
}
