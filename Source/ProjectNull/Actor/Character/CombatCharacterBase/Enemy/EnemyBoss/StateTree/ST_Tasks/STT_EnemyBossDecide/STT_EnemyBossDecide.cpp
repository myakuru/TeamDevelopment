#include "STT_EnemyBossDecide.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossDecide::USTT_EnemyBossDecide(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossDecide::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	//// どれかポインタがなければリターン
	//if (!IsValid(OwnerBoss) || !IsValid(TargetActor) || !IsValid(AIC))
	//{
	//	return EStateTreeRunStatus::Failed;
	//}

	//// 距離を算出
	//const float Distance = FVector::Dist(OwnerBoss->GetActorLocation(),TargetActor->GetActorLocation());

	//// 距離が攻撃範囲より狭ければ次のステートへ
	//if (Distance <= CurrentAttack.AttackRange)
	//{
	//	AIC->StopMovement();
	//	UE_LOG(LogTemp, Warning, TEXT("Approach: Dist=%f Range=%f"), Distance, CurrentAttack.AttackRange);
	//	return EStateTreeRunStatus::Succeeded;
	//}

	//// 速度をとりあえず決める
	//float Speed = CurrentAttack.SlowSpeed;

	//// 間合いに入ったら一気に距離を詰める
	//if (CurrentAttack.bUseFastSpeed && Distance <= CurrentAttack.FastStartDistance)
	//{
	//	Speed = CurrentAttack.FastSpeed;
	//}

	//// キャラクターのムーブコンポーネントに速度を代入
	//if (UCharacterMovementComponent* MoveComp = OwnerBoss->GetCharacterMovement())
	//{
	//	MoveComp->MaxWalkSpeed = Speed;
	//}

	//// 移動処理
	//UAIBlueprintHelperLibrary::SimpleMoveToActor(AIC, TargetActor);

	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossDecide::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UE_LOG(LogTemp, Warning, TEXT("Decide EnterState In"));

	Super::EnterState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// 距離を算出
	const float Dist = FVector::Dist(Boss->GetActorLocation(), TargetActor->GetActorLocation());
	// 歩き状態を選択
	EBossActionType BossAction = EBossActionType::ApproachWalk;
	// ボスの攻撃範囲と距離を比較
	if (Dist <= Boss->GetNearRange())
	{
		// 近距離
		if (FMath::FRand() < Boss->GetStrafeChance())
		{
			UE_LOG(LogTemp, Warning, TEXT("BossAction Strafe"));
			BossAction = EBossActionType::Strafe;		// 様子見
		}
		else if (Boss->SelectAttackByDistance(Dist))
		{
			UE_LOG(LogTemp, Warning, TEXT("BossAction Attack"));
			BossAction = EBossActionType::PlayAttack;		// 攻撃
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("BossAction Strafe"));
			BossAction = EBossActionType::Strafe;		// 条件に合致しなければ様子見
		}
	}
	else
	{
		// 遠距離
		// 歩く、走る、ジャンプ攻撃、遠距離攻撃から選ぶ
		// とりあえずランダムで作る
		const EBossActionType Far[] =
		{
			EBossActionType::ApproachWalk,
			EBossActionType::ApproachRun,
			EBossActionType::JumpAttack,
			EBossActionType::RangedAttack
		};
		BossAction = Far[FMath::RandRange(0, 3)];
		UE_LOG(LogTemp, Warning, TEXT("BossAction Range"));
	}

	Boss->SetNextAction(BossAction);

	UE_LOG(LogTemp, Warning, TEXT("Decide EnterState Out"));

	return EStateTreeRunStatus::Succeeded;
}

void USTT_EnemyBossDecide::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
