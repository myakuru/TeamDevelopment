#include "STT_EnemyBossIdle.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "GameFramework/Actor.h"

USTT_EnemyBossIdle::USTT_EnemyBossIdle(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossIdle::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	//UE_LOG(LogTemp, Warning, TEXT("IdleIn"));

	if (!IsValid(OwnerBoss))
	{
		return EStateTreeRunStatus::Failed;
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyBossIdle::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("IdleState"));
	//UE_LOG(LogTemp, Warning, TEXT("IdleUpdate"));
	if (!IsValid(OwnerBoss))
	{
		//UE_LOG(LogTemp, Warning, TEXT("OwnerBoss nullptr"));
		return EStateTreeRunStatus::Failed;
	}

	IdleCounter += DeltaTime;

	if (IdleCounter >= IdleDuration) 
	{
		//UE_LOG(LogTemp, Warning, TEXT("IdleSucceeded"));
		return EStateTreeRunStatus::Succeeded;
	}

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// プレイヤーの方向を向いておく
	FVector Dir = TargetActor->GetActorLocation() - Boss->GetActorLocation();
	Dir.Z = 0.0f;
	Dir.Normalize();

	const FRotator Want(0.0f, Dir.Rotation().Yaw, 0.0f);
	const FRotator Now = Boss->GetActorRotation();
	Boss->SetActorRotation(FMath::RInterpTo(Now, Want, DeltaTime, 10.0f));


	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossIdle::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	//UE_LOG(LogTemp, Warning, TEXT("IdleExit"));

	if (IsValid(AIC))
	{
		AIC->StopMovement();
	}
}

