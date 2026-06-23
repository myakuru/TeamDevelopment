
#include "ModelAfterimageTrailEffect.h"

#include <ProjectNull/Actor/GhostActor/GhostActor.h>

UModelAfterimageTrailEffect::UModelAfterimageTrailEffect():
	bEnableSpawn(false),
	GhostActors(TArray<TWeakObjectPtr<AGhostActor>>()),
	TrailAddTimer(0.0f),
	SpawnInterval(1.0f),
	TrailMaxLength(1),
	GhostClass(nullptr)
{
}

void UModelAfterimageTrailEffect::Update(
	float DeltaTime,
	const FTransform& InTransform,
	USkeletalMesh* InSkeletalMesh,
	const FPoseSnapshot& InSnapshot)
{
	UpdateInternal(
		DeltaTime,
		InTransform,
		InSkeletalMesh,
		[this, InSkeletalMesh, InSnapshot](AGhostActor* Ghost)
		{
			Ghost->Initialize(InSkeletalMesh, InSnapshot);
		});
}

void UModelAfterimageTrailEffect::Update(
	float DeltaTime,
	const FTransform& InTransform,
	USkeletalMesh* InSkeletalMesh,
	UAnimationAsset* InAnimation,
	float InPoseTime)
{
	UpdateInternal(
		DeltaTime,
		InTransform,
		InSkeletalMesh,
		[this, InSkeletalMesh, InAnimation, InPoseTime](AGhostActor* Ghost)
		{
			Ghost->Initialize(InSkeletalMesh, InAnimation, InPoseTime);
		});
}

void UModelAfterimageTrailEffect::AllDestroy()
{
	for (auto& Data : GhostActors)
	{
		if (!Data.IsValid()) { continue; }
		Data->Destroy();
	}

	GhostActors.Empty();
}


void UModelAfterimageTrailEffect::UpdateInternal(
	float DeltaTime,
	const FTransform& InTransform,
	USkeletalMesh* InSkeletalMesh,
	TFunction<void(AGhostActor*)> InitializeFunc)
{
	if (!InSkeletalMesh) { return; }

	if (!bEnableSpawn)
	{
		TrailAddTimer = 0.0f;
		return;
	}

	// 時間加算処理
	TrailAddTimer += DeltaTime;

	// 追加間隔に基づいて残像の追加を行う
	if (TrailAddTimer >= SpawnInterval)
	{
		// 残像の追加
		AddAfterimageTrail(
			InTransform,
			InitializeFunc);
		// 時間リセット
		TrailAddTimer = 0.0f;
	}

	// 最大保持数を超えた残像を削除
	DestroyOverflowTrail();
}

void UModelAfterimageTrailEffect::AddAfterimageTrail(
	const FTransform& InTransform,
	TFunction<void(AGhostActor*)> InitializeFunc)
{
	if (!bEnableSpawn) { return; }

	// 残像をワールドにスポーン
	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }

	// 初期化処理
	InitializeFunc(Ghost);
	Ghost->SetActorTransform(InTransform);

	// 配列の先頭に追加
	GhostActors.Insert(Ghost, 0);
}

void UModelAfterimageTrailEffect::DestroyOverflowTrail()
{
	// 残像の最大数を超えているとき、先頭から要素を削除
	// ※ワールドからも削除を行う
	while (GhostActors.Num() > TrailMaxLength)
	{
		auto* PopData = GhostActors.Pop().Get();
		
		if (PopData)
		{
			PopData->Destroy();
		}
	}
}

