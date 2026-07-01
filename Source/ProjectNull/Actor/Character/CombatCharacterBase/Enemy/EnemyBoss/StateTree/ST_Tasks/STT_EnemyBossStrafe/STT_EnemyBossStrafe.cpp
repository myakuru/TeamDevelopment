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
	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	Elapsed += DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("Strafe Tick Elapsed %f"), Elapsed);
	if (Elapsed >= StrafeTime)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Strafe Tick End"));
		AIC->StopMovement();
		return EStateTreeRunStatus::Succeeded;
	}

	// 距離が離れていればDicideステートに戻る
	if (GetOutDistance * GetOutDistance <= FVector::DistSquared(TargetActor->GetActorLocation(), Boss->GetActorLocation()))
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

	FVector MoveDir = FVector::ZeroVector;

	// 後ろ向きに移動するかしないかの分岐
	if (bBackStep)
	{
		// 後退はボス自身の後ろの方向
		Boss->SetActorRotation(FRotator(0.0f, ToPlayer.Rotation().Yaw, 0.0f));
		MoveDir = -Boss->GetActorForwardVector();
	}
	else
	{
		const FVector BossLocation = Boss->GetActorLocation();
		const FVector TargetLocation = TargetActor->GetActorLocation();

		// プレイヤーからボスの方向
		FVector FromTarget = BossLocation - TargetLocation;
		FromTarget.Z = 0.0f;

		const float CurrentRadius = FromTarget.Size();

		if (CurrentRadius <= KINDA_SMALL_NUMBER)
		{
			return EStateTreeRunStatus::Running;
		}

		const FVector RadialDir = FromTarget / CurrentRadius;

		// プレイヤーの周りをまわる接線方向
		FVector TangentDir = FVector::CrossProduct(FVector::UpVector, RadialDir).GetSafeNormal();

		// 左右反転
		TangentDir *= StrifeDir;

		// 半径を保つ補正
		// 近すぎるなら外へ、遠すぎるならうちへ
		const float RadiusError = OrbitRadius - CurrentRadius;
		const float CorrectionScale = FMath::Clamp(RadiusError / OrbitRadius, -1.0f, 1.0f);

		const FVector RadiusCorrection = RadialDir * CorrectionScale * OrbitRadiusCorrection;

		MoveDir = (TangentDir + RadiusCorrection).GetSafeNormal();

		// 向きはプレイヤーへ
		const FRotator Want(0, ToPlayer.Rotation().Yaw, 0);
		Boss->SetActorRotation(FMath::RInterpTo(Boss->GetActorRotation(), Want, DeltaTime, 8.0f));
	}

	Boss->GetCharacterMovement()->MaxWalkSpeed = StrafeSpeed;
	Boss->AddMovementInput(MoveDir, 1.0f);	// StrifeDirは１かー１

	//UE_LOG(LogTemp, Warning, TEXT("Strafe Tick Out"));

	return EStateTreeRunStatus::Running;
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossStrafe::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	Elapsed = 0.0f;

	int flg = FMath::RandBool();
	int bBackStepFlg = FMath::RandBool();
	int SelectAttack = 0;
	bBackStep = false;
	if (flg)
	{
		//StrifeDir = 1.0f;
		StrifeDir = -1.0f;
		SelectAttack = 1;
		//UE_LOG(LogTemp, Warning, TEXT(" In Strafe Right"));
	}
	else
	{
		//StrifeDir = -1.0f;
		StrifeDir = 1.0f;
		SelectAttack = 0;
		//UE_LOG(LogTemp, Warning, TEXT(" In Strafe Left"));
	}

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	UAnimInstance* Anim = Boss->GetMesh() ? Boss->GetMesh()->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	if (InsiteDistance * InsiteDistance >= FVector::DistSquared(TargetActor->GetActorLocation(), Boss->GetActorLocation()))
	{
		if (bBackStepFlg)
		{
			bBackStep = true;
			SelectAttack = 2;
			//UE_LOG(LogTemp, Warning, TEXT(" In Strafe Back"));
		}
	}

	Anim->Montage_Play(Boss->GetCurrentAttack().AttackMontages[SelectAttack]);

	// 永続再生
	Anim->Montage_JumpToSection(
		StrafeLoopSectionName,
		Boss->GetCurrentAttack().AttackMontages[SelectAttack]);

	Anim->Montage_SetNextSection(
		StrafeLoopSectionName,
		StrafeLoopSectionName,
		Boss->GetCurrentAttack().AttackMontages[SelectAttack]);

	// CharacterMovement の自動回転を切る
	if (UCharacterMovementComponent* Move = Boss->GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = false;
		Move->bUseControllerDesiredRotation = false;
		Move->MaxWalkSpeed = StrafeSpeed;
	}

	Boss->bUseControllerRotationYaw = false;

	//UE_LOG(LogTemp, Warning, TEXT(">>> Strafe ENTER"));

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossStrafe::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss))
	{
		return;
	}

	Boss->SetPrevAction(Boss->GetCurrentAction());

	// CharacterMovement の自動回転を有効化
	if (UCharacterMovementComponent* Move = Boss->GetCharacterMovement())
	{
		Move->bOrientRotationToMovement = true;
		Move->bUseControllerDesiredRotation = true;
		Move->MaxWalkSpeed = StrafeSpeed;
	}

	Boss->bUseControllerRotationYaw = true;
}
