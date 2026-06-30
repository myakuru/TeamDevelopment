#include "STT_EnemyBossStrafe.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossStrafe::USTT_EnemyBossStrafe(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossStrafe::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Strafe Tick In"));

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	Elapsed += DeltaTime;
	UE_LOG(LogTemp, Warning, TEXT("Strafe Tick Elapsed %f"), Elapsed);
	if (Elapsed >= StrafeTime)
	{
		UE_LOG(LogTemp, Warning, TEXT("Strafe Tick End"));
		AIC->StopMovement();
		return EStateTreeRunStatus::Succeeded;
	}

	// 距離が離れていればDicideステートに戻る
	if (GetOutDistance <= FVector::DistSquared(TargetActor->GetActorLocation(), Boss->GetActorLocation()))
	{
		AIC->StopMovement();
		return EStateTreeRunStatus::Succeeded;
	}

	// ターゲットの方向のベクトルを取得
	FVector ToPlayer = TargetActor->GetActorLocation() - Boss->GetActorLocation();
	ToPlayer.Z = 0.0f;
	ToPlayer.Normalize();
	// 横方向のベクトルを取得
	const FVector RightVector = FVector::CrossProduct(FVector::UpVector, ToPlayer);
	Boss->GetCharacterMovement()->MaxWalkSpeed = StrafeSpeed;
	Boss->AddMovementInput(RightVector * StrifeDir, 1.0f);	// StrifeDirは１かー１

	// 顔はプレイヤーへ
	const FRotator Want(0, ToPlayer.Rotation().Yaw, 0);
	Boss->SetActorRotation(FMath::RInterpTo(Boss->GetActorRotation(), Want, DeltaTime, 8.0f));

	UE_LOG(LogTemp, Warning, TEXT("Strafe Tick Out"));

	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossStrafe::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	Elapsed = 0.0f;

	int flg = FMath::RandBool();
	int SelectAttack = 0;
	if (flg)
	{
		StrifeDir = 1.0f;
		SelectAttack = 1;
	}
	else
	{
		StrifeDir = -1.0f;
		SelectAttack = 0;
	}
	//StrifeDir = (FMath::RandBool()) ? 1.0f : -1.0f;

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	Anim->Montage_Play(Boss->GetCurrentAttack().AttackMontages[SelectAttack]);

	UE_LOG(LogTemp, Warning, TEXT(">>> Strafe ENTER"));

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossStrafe::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
	UE_LOG(LogTemp, Warning, TEXT("<<< Strafe EXIT"));
}
