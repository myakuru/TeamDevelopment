#include "EnemyISMManager.h"

#include "Components/InstancedStaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/Animation/AnimDataAsset.h>

AEnemyISMManager::AEnemyISMManager()
{
	PrimaryActorTick.bCanEverTick = false;	// Tickは必要ないので無効化
	// RootComponentとしてISMを生成
	ISM = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ISM"));
	RootComponent = ISM;
	ISM->NumCustomDataFloats = NumCustomDataFloats;
}

void AEnemyISMManager::BeginPlay()
{
	Super::BeginPlay();
	
	// エディタで設定したメッシュをISMに適用する
	if (EnemyMesh)
	{
		ISM->SetStaticMesh(EnemyMesh);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] EnemyMesh Null : %s"), *GetName());
	}

	// マテリアルに渡すCustomDataのチャンネル数を設定する
	// AnimToTextureのAnimTime・AnimIndexを渡すために最低２チャンネル必要
	//ISM->NumCustomDataFloats = NumCustomDataFloats;

	// 敵管理サブシステムに自身を登録する
	if (auto* Subsystem = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>())
	{
		Subsystem->RegisterISMManager(this);
	}
}

int32 AEnemyISMManager::AllocateIndex()
{
	if (FreeIndices.Num() > 0)
	{
		// 削除で空いた穴を再利用することでインスタンス総数を増やさない
		return FreeIndices.Pop();
	}

	// 穴がない場合のみ末尾にインスタンスを追加する
	ISM->AddInstance(FTransform::Identity);
	return MaxInstanceCount++;
}

void AEnemyISMManager::ReleaseIndex(int32 Index)
{
	// RemoveInstanceを呼ぶと後ろのインデックスがずれるため呼ばない
	// 代わりにスケール0で負荷しにしてFreeListに積む
	FTransform Hidden;
	Hidden.SetScale3D(FVector::ZeroVector);
	ISM->UpdateInstanceTransform(Index, Hidden, true, false);

	FreeIndices.Add(Index);

	// 描画を更新してインスタンスの削除を反映させる
	ISM->MarkRenderStateDirty();
}

void AEnemyISMManager::RegisterEnemy(AEnemyBase* Enemy)
{
	if (!IsValid(Enemy))
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] RegisterEnemy: Enemy is invalid"));
		return;
		//return INDEX_NONE;
	}

	// 既に登録済みの場合は一度解除してから再登録する
	// Activate→Deactivate→Activateのサイクルで
	// 前回のインデックスが残っている場合の対処
	if (Enemy->ISMInstanceIndex != INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ISMManager] RegisterEnemy: 既存インデックスを解除して再登録: %d"), Enemy->ISMInstanceIndex);
		ReleaseIndex(Enemy->ISMInstanceIndex);
		Enemy->ISMInstanceIndex = INDEX_NONE;
	}

	FTransform AdjustTransform = Enemy->GetActorTransform();
	// 90度回転させる
	FRotator Rotation = AdjustTransform.GetRotation().Rotator();
	Rotation.Yaw -= 90.0f;
	AdjustTransform.SetRotation(Rotation.Quaternion());

	FVector Location = AdjustTransform.GetLocation();
	Location.Z -= 90.0f;
	AdjustTransform.SetLocation(Location);


	// 空いているインデックスを確保してEnemyに持たせる
	// Enemy自身がインデックスを知っているのでマイフレームのFindが不要になる
	int32 Index = AllocateIndex();
	Enemy->ISMInstanceIndex = Index;
	
	ISM->UpdateInstanceTransform(
		Index,
		AdjustTransform,
		true,
		false	// MarkRenderStateDirtyをfalseにしてレンダリング状態の更新を遅延させる
	);

	const int AnimIndex = Enemy->GetAnimIndex();
	const FAnimData& AnimData = AnimDataAsset->Animations[AnimIndex];
	ISM->SetCustomDataValue(Index, 0, 0.0f);
	ISM->SetCustomDataValue(Index, 2, AnimIndex);
	ISM->SetCustomDataValue(Index, 3, AnimData.StartTime);
	ISM->SetCustomDataValue(Index, 4, AnimData.NumFrames);
	ISM->SetCustomDataValue(Index, 5, 0.0f);
	ISM->SetCustomDataValue(Index, 6, AnimData.StartTime);
	ISM->SetCustomDataValue(Index, 7, 0.0f);

	Enemies.Add(Enemy);
}

void AEnemyISMManager::UnregisterEnemy(AEnemyBase* Enemy)
{
	if(!IsValid(Enemy))
	{
		//UE_LOG(LogTemp, Warning, TEXT("[EnemyISMManager] UnregisterEnemy: Enemy is invalid"));
		return;
	}

	// INDEX_NONEは未登録またはすでに解除済みを意味する
	// 二重登録を防ぐためチェック
	if (Enemy->ISMInstanceIndex == INDEX_NONE) { return; }

	// インスタンスをISMから解除してインデックスを解放する
	ReleaseIndex(Enemy->ISMInstanceIndex);

	// インスタンスを解放してEnemyからインデックスをクリアする
	Enemy->ISMInstanceIndex = INDEX_NONE;
	Enemies.Remove(Enemy);
}

void AEnemyISMManager::UpdateEnemies(float DeltaTime)
{
	for (AEnemyBase* Enemy : Enemies)
	{

		if (!IsValid(Enemy) || Enemy->ISMInstanceIndex == INDEX_NONE) { continue; }

		const int AnimIndex = Enemy->GetAnimIndex();
		const int NextAnimIndex = Enemy->GetNextAnimIndex();
		const FAnimData& AnimData = AnimDataAsset->Animations[AnimIndex];
		const FAnimData& NextAnimData = AnimDataAsset->Animations[NextAnimIndex];

		// SampleRate = 30fps、NumFrames はアニメーションのフレーム総数
		const float SampleRate = 30.0f;
		const float NumFrames = AnimDataAsset->Animations[AnimIndex].NumFrames - AnimData.StartTime; // AnimToTextureベイク時のフレーム数に合わせる
		// AnimTimeからフレーム番号を計算する
		// 前フレームと今フレームでループをまたいだか判定
		const float PrevRawFrame = Enemy->GetBeginAnimTime() * SampleRate;
		const float CurrRawFrame = Enemy->GetAnimTime() * SampleRate;
		const float CurrentFrame = FMath::Fmod(CurrRawFrame, NumFrames);
		const float PrevFrame = FMath::Fmod(Enemy->GetBeginAnimTime() * SampleRate, NumFrames);

		// Channel 6 : 次アニメのフレーム番号
		//const float NextFrameStart = NextAnimData.StartTime + Enemy->GetNextAnimTime() * SampleRate;
		const float NextNumFrames = NextAnimData.NumFrames/* - NextAnimData.StartTime*/;
		const float NextFrame = FMath::Fmod(Enemy->GetNextAnimTime() * SampleRate, NextNumFrames);

		if ((int)(PrevRawFrame / NumFrames) < (int)(CurrRawFrame / NumFrames))
		{
			// 0なら１，１なら０にするだけ
			Enemy->AnimChangeFlg = true;
			int32 TargetAnim = (AnimIndex == 1) ? 0 : 1;
			Enemy->SetNextAnimIndex(TargetAnim);
		}

		// オフセット適用
		FTransform AdjustTransform = Enemy->GetActorTransform();
		// 90度回転させる
		FRotator Rotation = AdjustTransform.GetRotation().Rotator();
		Rotation.Yaw -= 90.0f;
		AdjustTransform.SetRotation(Rotation.Quaternion());

		FVector Location = AdjustTransform.GetLocation();
		Location.Z -= 90.0f;
		AdjustTransform.SetLocation(Location);

		// ActorのTransformをISMに反映
		// bMarkRenderStateDirtyをfalseにして最後にまとめてDirtyを立てる
		ISM->UpdateInstanceTransform(
			Enemy->ISMInstanceIndex,
			AdjustTransform,
			true,
			false	// MarkRenderStateDirtyをfalseにしてレンダリング状態の更新を遅延させる
		);

		// AnimToTextureのパラメータをマテリアルに渡す
		// Channel 0 : CurrentFrame - アニメーションの現在の再生時間
		// Channel 1 : PrevFrame - 前フレームのアニメーション
		// Channel 2 : AnimIndex - 現在再生中のアニメーションのインデックス
		//						  （どのアニメーションを再生するか）
		// Channel 3 : StartTime - 次のアニメーションの始まる時間
		//						  （マテリアルノードで前のアニメーションのNumFramesに加算して、次のアニメーションの始まりを指定）
		// Channel 4 : NumFrames - そのアニメーションまでの時間の総合フレーム
		// アニメーションブレンド用
		// Channel 5 : Flg - アニメーションブレンドフラグ
		// Channel 6 : 次のアニメーションの開始時間
		// Channel 7 : アニメーションブレンドの進行割合
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 0, CurrentFrame);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 1, PrevFrame);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 2, AnimIndex);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 3, AnimData.StartTime);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 4, AnimData.NumFrames);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 5, static_cast<float>(Enemy->AnimChangeFlg));
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 6, NextFrame);
		ISM->SetCustomDataValue(Enemy->ISMInstanceIndex, 7, Enemy->GetAnimBlendWeight());

		/*UE_LOG(LogTemp, Warning,
			TEXT("CurrentFrame:%.3f | PrevFrame:%.3f | AnimDataStartTime:%.3f | AnimDataNumFrames:%.3f | NextFrame:%.3f"),
			CurrentFrame, PrevFrame, AnimData.StartTime, AnimData.NumFrames, NextFrame);
		UE_LOG(LogTemp, Warning,
			TEXT("X:%.3f | Y:%.3f | Z:%.3f"),
			Enemy->GetActorLocation().X, Enemy->GetActorLocation().Y, Enemy->GetActorLocation().Z);
		UE_LOG(LogTemp, Warning,TEXT("ISMIndex:%d"), Enemy->ISMInstanceIndex);*/
	}

	if (Enemies.Num() > 0)
	{
		ISM->MarkRenderStateDirty(); // すべての更新が完了した後にレンダリング状態を更新
	}
}