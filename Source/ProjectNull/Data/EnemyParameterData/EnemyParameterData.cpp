#include "EnemyParameterData.h"

UEnemyParameterData::UEnemyParameterData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
{
}

void UEnemyParameterData::CalcDistanceToTarget(const FVector& a_TargetPos, const FVector& a_OwnerPos)
{
	// ターゲット座標・移動方向・簡易距離を算出
	MoveDir				= a_TargetPos - a_OwnerPos;
	TargetDistanceSqr	= MoveDir.SizeSquared();

	// デリゲート内のセッターを発火
	OnTargetDistChanged.Broadcast(MoveDir, TargetDistanceSqr);

	/* 備忘録 */
	// クラスをUObjectを継承したEnemyRunTimeParameterに変更する
	// この後EnemyBase側でUEnemyParameterDataを持たせる
	// EnemyBase側で、
	//・UEnemyParameterDataのゲッター
	//・進行ベクトル・距離のセッター
	// それぞれを用意し、セッターはデリゲートに追加
	// STE側にセッターを用意して同じくデリゲートに追加
}