#include "STT_EnemyKnockBack.h"
#include <ProjectNull\System\DataTable\KnockBackData\KnockBackData.h>

USTT_EnemyKnockBack::USTT_EnemyKnockBack(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
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

	/* 備忘録 */
	// ・KnockBackFlagが有効の時に入る
	// ・デリゲートを用いて、フラグを管理する
	// ・Enemy側にデリゲート発火用ラップ関数を作成
	
	SetKnockBackData();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyKnockBack::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyKnockBack::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
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

	// 速度・ノックバックフラグ・攻撃不可能フラグ
	KnockBackVelocity = LanchDir * KnockBackData->LaunchSpeed;

}
