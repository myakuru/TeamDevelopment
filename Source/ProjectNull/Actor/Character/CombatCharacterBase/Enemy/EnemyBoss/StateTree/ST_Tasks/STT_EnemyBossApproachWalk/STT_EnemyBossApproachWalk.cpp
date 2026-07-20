#include "STT_EnemyBossApproachWalk.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossApproachWalk::USTT_EnemyBossApproachWalk(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossApproachWalk::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("ApproachWalk Tick In"));
	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	if (ApproachChangeCount >= ApproachChangeDuration)
	{
		NextApproachCurrentData();
		return EStateTreeRunStatus::Succeeded;
	}

	// 距離を算出
	const float Distance = FVector::Dist(Boss->GetActorLocation(),TargetActor->GetActorLocation());

	// 距離が攻撃範囲より狭ければ次のステートへ
	if (Distance <= Boss->GetNearRange())
	{
		NextApproachCurrentData();
		return EStateTreeRunStatus::Succeeded;

		//UE_LOG(LogTemp, Warning, TEXT("ApproachWalk Succeeded"));
	}

	// キャラクターのムーブコンポーネントに速度を代入
	if (UCharacterMovementComponent* MoveComp = OwnerBoss->GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = MoveSpeed;
		//UE_LOG(LogTemp, Warning, TEXT("ApproachWalk Move"));
	}

	// 移動処理
	UAIBlueprintHelperLibrary::SimpleMoveToActor(AIC, TargetActor);

	//UE_LOG(LogTemp, Warning, TEXT("ApproachWalk Tick Out"));

	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossApproachWalk::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossApproachWalk::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}

void USTT_EnemyBossApproachWalk::NextApproachCurrentData()
{
	ApproachChangeCount = 0.0f;

	AIC->StopMovement();
}
