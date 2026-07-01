#include "STT_EnemyBossDecide.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
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
	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossDecide::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	//UE_LOG(LogTemp, Warning, TEXT("Decide EnterState In"));

	Super::EnterState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// 距離を算出
	const float Dist = FVector::Dist(Boss->GetActorLocation(), TargetActor->GetActorLocation());
	// 歩き状態を選択
	EBossActionType BossAction = EBossActionType::ApproachWalk;

	// 優先度の高いものがある場合それを選択する
	if (Boss->GetActionPriority() != EBossActionType::None)
	{
		Boss->SetCurrentAction(Boss->GetActionPriority());
		Boss->SelectNextAttack(Boss->GetActionPriority());
	}
	else
	{
		// ボスの攻撃範囲と距離を比較
		bool DistRange = (Dist <= Boss->GetNearRange());

		TArray<FBossActionEntry>& ActionEntry =
			DistRange ? ActionTable->NearActions
			: ActionTable->FarActions;

		// 同じモーションに連続で入らないようにする
		for (int i = 0; i < 10; ++i) 
		{
			BossAction = SelectWeightedRandom(ActionEntry);
			if (BossAction != Boss->GetPrevAction()) 
			{ break; }
		}
		Boss->SetCurrentAction(BossAction);
		Boss->SelectNextAttack(BossAction);

		//if (Dist <= Boss->GetNearRange())
		//{

		//	// 近距離
		//	if (FMath::FRand() < Boss->GetStrafeChance())
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("BossAction Strafe"));
		//		BossAction = EBossActionType::Strafe;		// 様子見
		//	}
		//	else if (Boss->SelectAttackByDistance(Dist))
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("BossAction Attack"));
		//		BossAction = EBossActionType::PlayAttack;	// 攻撃
		//	}
		//	else
		//	{
		//		UE_LOG(LogTemp, Warning, TEXT("BossAction Strafe"));
		//		BossAction = EBossActionType::Strafe;		// 条件に合致しなければ様子見
		//	}
		//}
		//else
		//{
		//	// 遠距離
		//	// 歩く、走る、ジャンプ攻撃、遠距離攻撃から選ぶ
		//	// とりあえずランダムで作る
		//	const EBossActionType Far[] =
		//	{
		//		EBossActionType::ApproachWalk,
		//		EBossActionType::ApproachRun,
		//		EBossActionType::JumpAttack,
		//		EBossActionType::RangedAttack
		//	};
		//	BossAction = Far[FMath::RandRange(0, 3)];
		//	if (BossAction == EBossActionType::JumpAttack || BossAction == EBossActionType::RangedAttack)
		//	{
		//		Boss->SelectAttackByDistance(Dist);
		//	}
		//	UE_LOG(LogTemp, Warning, TEXT("BossAction Range"));
		//}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Decide EnterState Out"));

	return EStateTreeRunStatus::Succeeded;
}

void USTT_EnemyBossDecide::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}


EBossActionType USTT_EnemyBossDecide::SelectWeightedRandom(const TArray<FBossActionEntry>& Entries)
{
	// EntriesがなければWalkを返す
	if (Entries.IsEmpty()) return EBossActionType::ApproachWalk;

	// 配列のWeightを足してトータルWeightを作る
	float TotalWeight = 0.0f;
	for (const auto& E : Entries) TotalWeight += E.Weight;

	// 0からTotalWeightから値を乱数で出力
	float Rand = FMath::FRandRange(0.0f, TotalWeight);
	float Cumulative = 0.0f;

	// Weightをもとに選択
	for (const auto& E : Entries)
	{
		// CumulativeにWeightを順番にタス
		Cumulative += E.Weight;
		// Cumulativeの値が算出した乱数を上回ったらそのActionTypeを返す
		if (Rand <= Cumulative) return E.ActionType;
	}

	// 配列からはみ出したらWalkを返す
	return EBossActionType::ApproachWalk;
}
