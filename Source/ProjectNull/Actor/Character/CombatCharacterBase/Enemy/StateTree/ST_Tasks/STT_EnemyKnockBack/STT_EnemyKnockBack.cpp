#include "STT_EnemyKnockBack.h"
#include "StateTreeExecutionContext.h"

#include <ProjectNull\Utility\Common\Definitions\CollisionChannels.h>
#include <ProjectNull\System\DataAssets\KnockBackData\KnockBackData.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>

USTT_EnemyKnockBack::USTT_EnemyKnockBack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	OwnerEnemy(nullptr)
	,	KnockBackData(nullptr)
	,	KnockBackVelocity(FVector::ZeroVector)
	,	MoveDir(FVector::ZeroVector)
	,	TargetLocation(FVector::ZeroVector)
	,	DamageRatio(0.0f)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyKnockBack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// ノックバックに必要な情報を取得・設定
	SetKnockBackData();

	// 敵同士の当たり判定を一時的に消す
	OwnerEnemy->NotifyChangedCollisionResponseToChannel(ECC_Enemy, ECollisionResponse::ECR_Ignore);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyKnockBack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	Super::Tick(a_Context, a_DeltaTime);

	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// ノックバックが停止したらステート終了
	if (MoveToKnockBack(a_DeltaTime)) 
	{
		// ステートタイプを切り替え
		OwnerEnemy->NotifyChangedStateEnum(EEnemyState::Idle);

		return EStateTreeRunStatus::Succeeded; 
	}

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyKnockBack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);

	if (!OwnerEnemy) { return; }

	// 敵同士の当たり判定を戻す
	OwnerEnemy->NotifyChangedCollisionResponseToChannel(ECC_Enemy, ECollisionResponse::ECR_Block);
}

void USTT_EnemyKnockBack::SetKnockBackData()
{
	if (!OwnerEnemy) { return; }

	// データアセットを取得
	if (!KnockBackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnockBackデータに必要なデータアセットがありません!!"));
		return;
	}

	// ノックバック速度をダメージ割合により算出
	float KnockBackSpeed = KnockBackData->KnockBackSpeed * DebugDamageRatio;

	// 水平方向(移動方向を反転)
	FVector HorizontalDir = OwnerEnemy->GetActorLocation() - TargetLocation;
	HorizontalDir.Z = 0;
	HorizontalDir.Normalize();

	// 水平方向へのノックバック速度
	const FVector HorizontalVelocity = HorizontalDir * KnockBackSpeed;

	// 上方向へのノックバック速度
	const FVector VerticalVelocity = FVector::UpVector * KnockBackSpeed;

	//// 吹き飛び角度
	//const float Rad = FMath::DegreesToRadians(KnockBackData->LaunchAngleDeg);
	//// 水平方向の角度とラジアン角を基に上向きの角度を作る
	//FVector LanchDir = HorizontalDir * FMath::Cos(Rad) + FVector::UpVector * FMath::Sin(Rad);
	//LanchDir.Normalize();

	// 速度を合成
	KnockBackVelocity = HorizontalVelocity + VerticalVelocity;
}

bool USTT_EnemyKnockBack::MoveToKnockBack(const float a_DeltaTime)
{
	if (!OwnerEnemy) { return false; }
	FVector CurrentLocation = OwnerEnemy->GetActorLocation();

	// 重力(-980.f)を速度に加算
	KnockBackVelocity.Z += GetWorld()->GetGravityZ() * a_DeltaTime;

	// 位置更新
	FVector NextLocation = CurrentLocation + KnockBackVelocity * a_DeltaTime;

	// どこかに当たったら停止
	FHitResult HitResult;
	OwnerEnemy->SetActorLocation(NextLocation, true, &HitResult);
	if (HitResult.GetComponent() &&
		HitResult.GetComponent()->GetCollisionObjectType() == ECC_WorldStatic)
	{
		return true;
	}

	return false;
}
