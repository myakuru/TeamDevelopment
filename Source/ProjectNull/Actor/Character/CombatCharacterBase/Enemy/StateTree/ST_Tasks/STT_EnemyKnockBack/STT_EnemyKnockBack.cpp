#include "STT_EnemyKnockBack.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\System\DataTable\KnockBackData\KnockBackData.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull/Data/CharacterRuntimeData/EnemyRuntimeData/EnemyRuntimeData.h>

USTT_EnemyKnockBack::USTT_EnemyKnockBack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	OwnerEnemy(nullptr)
	,	KnockBackDataTable(nullptr)
	,	KnockBackVelocity(FVector::ZeroVector)
	,	MoveDir(FVector::ZeroVector)
	,	ReceivedAttackPower(0.0f)
	,	EnemyWeight(0.0f)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyKnockBack::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// 前ステートの終了フラグをリセット
	OwnerEnemy->GetEnemyRuntimeData()->ResetAnimFinished();
	// 再生したいアニメを設定（インデックス・ループOFF・ブレンド開始）
	OwnerEnemy->GetEnemyRuntimeData()->SetNextAnimData(static_cast<uint32>(EEnemyState::Hit), false, true);

	OwnerEnemy->PlayAnimation(1, false);

	// ノックバックに必要な情報を取得・設定
	SetKnockBackData();
	
	// 敵同士の当たり判定を一時的に消す
	OwnerEnemy->NotifyChangedCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyKnockBack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	Super::Tick(a_Context, a_DeltaTime);

	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// アニメが1周したらSucceededを返してStateTreeに遷移を委ねる
	/*if (OwnerEnemy->GetEnemyRuntimeData()->GetAnimFinished())
	{
		return EStateTreeRunStatus::Succeeded;
	}*/

	// ノックバックが停止したらステート終了
	if(MoveToKnockBack(a_DeltaTime)){ return EStateTreeRunStatus::Succeeded; }

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyKnockBack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);

	if (!OwnerEnemy) { return; }

	// ステートタイプを切り替え
	OwnerEnemy->NotifyChangedStateEnum(EEnemyState::Idle);

	// 敵同士の当たり判定を戻す
	OwnerEnemy->NotifyChangedCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

void USTT_EnemyKnockBack::SetKnockBackData()
{
	// 吹き飛ばしに使う数値を決定
	int32 KnockBackPowerLevel = ReceivedAttackPower - EnemyWeight;
	if (KnockBackPowerLevel < 0)
	{
		KnockBackPowerLevel = 0;
	}

	const FName RowName = FName(*FString::FromInt(KnockBackPowerLevel));

	// RowNameから型付きでで取得
	const FKnockBackData* KnockBackData =
		KnockBackDataTable->FindRow<FKnockBackData>(RowName, TEXT("KnockBack"));
	if (!KnockBackData)
	{
		UE_LOG(LogTemp, Warning, TEXT("KnockBack row not found: %s"), *RowName.ToString());
		return;
	}

	// 水平方向(移動方向を反転)
	FVector HorizontalDir = -MoveDir;

	// 吹き飛び角度
	const float Rad = FMath::DegreesToRadians(KnockBackData->LaunchAngleDeg);
	// 水平方向の角度とラジアン角を基に上向きの角度を作る
	FVector LanchDir = HorizontalDir * FMath::Cos(Rad) + FVector::UpVector * FMath::Sin(Rad);
	LanchDir.Normalize();

	// 速度をセット
	KnockBackVelocity = LanchDir * KnockBackData->LaunchSpeed;
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
