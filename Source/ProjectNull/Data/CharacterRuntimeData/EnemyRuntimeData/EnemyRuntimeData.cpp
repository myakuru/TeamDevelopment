#include "EnemyRuntimeData.h"

UEnemyRuntimeData::UEnemyRuntimeData()
	:	MoveDir(FVector::ZeroVector)
	,	TargetDistanceSqr(0.0f)
	,	StateEnum(EEnemyState::Idle)
	,	IsAlive(true)
{
}

// 次のアニメーションに使用するデータをセット
void UEnemyRuntimeData::SetNextAnimData(int32 NextIndex, bool RoopFlg, bool ChangeFlg)
{
	NextAnimIndex = NextIndex;
	AnimRoopFlg = RoopFlg;
	AnimChangeFlg = ChangeFlg;
}

void UEnemyRuntimeData::UpdateAnimation(float DeltaTime, float BlendSpeed)
{
	/* 備忘録 */
	// アニメション進行度を知る
	// Executeの度にAnimFrameを更新
	// 

	PrevAnimTime = AnimTime;

	// アニメーションブレンド中はCurrentAnimTimeを止めてブレンドするため
	// アニメーションブレンド用のフラグがFalseの時のみAnimTimeを更新
	if (!AnimChangeFlg)
	{
		AnimTime += DeltaTime;
	}
	else
	{
		AnimBlendWeight += DeltaTime / BlendSpeed;
		NextAnimTime += DeltaTime;
		// 1.0fでクランプする
		AnimBlendWeight = FMath::Min(AnimBlendWeight, 1.0f);
	}
}

void UEnemyRuntimeData::ComplateAnimBlend()
{
	// ブレンド完了したら切り替える（最初ではなく最後）
	//if (AnimBlendWeight >= 1.0f)
	{
		AnimBlendWeight = 0.0f;
		AnimChangeFlg = false;

		// ここで初めてアニメ切り替え
		AnimIndex = NextAnimIndex;
		AnimTime = NextAnimTime;	// 1回だけリセット
		NextAnimTime = 0.0f;
	}
}

void UEnemyRuntimeData::AnimationReset()
{
	AnimTime = 0.0f;
	PrevAnimTime = 0.0f;

	AnimIndex = 0;
	NextAnimIndex = 0;
	NextAnimTime = 0.0f;

	AnimNumFrames = 0.0f;

	AnimBlendWeight = 0.0f;
}

void UEnemyRuntimeData::CalcDistanceToTarget(const FVector& a_TargetPos, const FVector& a_OwnerPos)
{
	// 移動方向・距離の二乗値を算出
	const FVector	NewMoveDir			= a_TargetPos - a_OwnerPos;
	const float		NewTargetDistSqr	= NewMoveDir.SizeSquared();
	
	// 進行方向に変化があればブロードキャスト発火
	if (!MoveDir.Equals(NewMoveDir))
	{
		MoveDir = NewMoveDir;
		MoveDir.Normalize();
		OnMoveDirChanged.Broadcast(MoveDir);
	}

	// 距離に変化があればブロードキャスト発火
	if (!FMath::IsNearlyEqual(TargetDistanceSqr, NewTargetDistSqr))
	{
		TargetDistanceSqr = NewTargetDistSqr;
		OnTargetDistChanged.Broadcast(TargetDistanceSqr);
	}
}

void UEnemyRuntimeData::ChangedEnemyState(EEnemyState a_StateEnum)
{
	if (StateEnum == a_StateEnum) { return; }

	// 変更があればブロードキャスト発火
	StateEnum = a_StateEnum;
	OnStateEnumChanged.Broadcast(StateEnum);
}

void UEnemyRuntimeData::ChangedIsAlive(const bool a_IsAlive)
{
	if (IsAlive == a_IsAlive) { return; }

	// 変更があればブロードキャスト発火
	IsAlive = a_IsAlive;
	OnIsAliveChanged.Broadcast(IsAlive);
}
