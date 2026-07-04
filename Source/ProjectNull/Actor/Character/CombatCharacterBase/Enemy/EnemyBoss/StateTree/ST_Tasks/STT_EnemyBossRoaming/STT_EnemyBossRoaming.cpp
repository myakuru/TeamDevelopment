#include "STT_EnemyBossRoaming.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "GameFramework/Actor.h"

USTT_EnemyBossRoaming::USTT_EnemyBossRoaming(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossRoaming::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	bHasDestination = false;

	if (!IsValid(OwnerBoss) || !IsValid(AIC))
	{
		return EStateTreeRunStatus::Failed;
	}

	PickNewDestination(); // 最初の目的地
	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyBossRoaming::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	if (!IsValid(OwnerBoss) || !IsValid(AIC))
	{
		return EStateTreeRunStatus::Failed;
	}

	// 目的地に到着していたら次の点へ
	if (bHasDestination)
	{
		const float DistSq = FVector::DistSquared(OwnerBoss->GetActorLocation(), CurrentDestination);
		if (DistSq <= FMath::Square(AcceptanceRadius))
		{
			PickNewDestination();
		}
	}
	else
	{
		PickNewDestination();
		//UE_LOG(LogTemp, Warning, TEXT("Roaming NavMesh Out"));
	}

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossRoaming::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	if (IsValid(AIC))
	{
		AIC->StopMovement();
	}
}

bool USTT_EnemyBossRoaming::PickNewDestination()
{
	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return false; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return false; }

	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Boss->GetWorld());
	if (!IsValid(NavSys))
	{
		bHasDestination = false;
		return false;
	}

	FNavLocation Result;
	if (NavSys->GetRandomReachablePointInRadius(Boss->GetActorLocation(), RoamRadius, Result))
	{
		CurrentDestination = Result.Location;
		bHasDestination = true;
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIC, CurrentDestination);
		return true;
	}

	bHasDestination = false;
	return false;
}

