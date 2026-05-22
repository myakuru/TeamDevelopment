
#include "AfterImageAttackEffect.h"

#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Effect/ModelAfterimageTrailEffect/ModelAfterimageTrailEffect.h>

UAfterImageAttackEffect::UAfterImageAttackEffect():
	AfterImageDataArray(TArray<FAfterImageAttackData>()),
	SkeletalMesh(nullptr),
	AnimationAsset(nullptr),
	StartTransfrom(FTransform())
{

}

void UAfterImageAttackEffect::Initialize()
{
}

void UAfterImageAttackEffect::Start(const FTransform& Transform)
{
	StartTransfrom = Transform;
}

void UAfterImageAttackEffect::Update(float DeltaTime, float ElapsedTime)
{

	for (auto& Data : AfterImageDataArray) 
	{
		// 制御時間外なら非表示にし、更新を行わない
		if(!Data.IsWithinTimeRange(ElapsedTime))
		{
			Data.SetEnableSpawn(false);
			continue;
		}

		// データの更新
		UpdateAfterimageAttackData(DeltaTime, ElapsedTime, Data);
	}
}

float UAfterImageAttackEffect::GetMaxTime()
{
	float MaxTime = 0.f;

	for (auto& Data : AfterImageDataArray) 
	{
		// 時間閾値 + 移動時間 を計算し制御終了時間の最大値を求める
		if (Data.TimeThreshold + Data.MoveTime > MaxTime) 
		{
			MaxTime = Data.TimeThreshold + Data.MoveTime;
		}
	}

	return MaxTime;
}

void UAfterImageAttackEffect::UpdateAfterimageAttackData(float DeltaTime, float ElapsedTime, FAfterImageAttackData& Data)
{
	// 残像を追加
	Data.SetEnableSpawn(true);

	// 開始時のトランフォーム情報から行列と座標を取得する
	const FVector Location	= StartTransfrom.GetLocation();
	const FMatrix Matrix	= StartTransfrom.ToMatrixWithScale();

	// 行列から各軸ベクトルを取得する
	const FVector Forward	= Matrix.GetUnitAxis(EAxis::X);
	const FVector Right		= Matrix.GetUnitAxis(EAxis::Y);
	const FVector Up		= Matrix.GetUnitAxis(EAxis::Z);

	// オフセット座標の計算
	const FVector Offset	= Data.CalcLocationOffset(Data.GetCurrentTime(ElapsedTime) / Data.MoveTime);

	// 開始時のトランフォーム情報とオフセットトランフォーム情報を考慮を計算
	const FVector ResultLocation = Location
		+ Forward * Offset.X
		+ Right * Offset.Y
		+ Up * Offset.Z;

	const FVector MoveDir	= Data.CalcMoveDir();
	const FVector ResultDir = { Forward * MoveDir.X + Right * MoveDir.Y + Up * MoveDir.Z };

	// 計算計算をトランフォーム情報に適用させる
	Data.Transform.SetLocation(ResultLocation);
	Data.Transform.SetRotation((ResultDir.Rotation() + Data.RotationOffset).Quaternion());
	Data.Transform.SetScale3D(Data.Scale);

	if (!Data.ModelAfterimageTrailEffect) { return; }

	Data.ModelAfterimageTrailEffect->Update(DeltaTime, Data.Transform, SkeletalMesh, AnimationAsset, Data.PoseTime);
}

void FAfterImageAttackData::SetEnableSpawn(bool bInEnableSpawn) const
{
	// フラグ変更可能なときのみ実行できる
	if (!ModelAfterimageTrailEffect
		|| ModelAfterimageTrailEffect->EnableSpawn() == bInEnableSpawn)
	{
		return;
	}

	ModelAfterimageTrailEffect->SetEnableSpawn(bInEnableSpawn);
}
