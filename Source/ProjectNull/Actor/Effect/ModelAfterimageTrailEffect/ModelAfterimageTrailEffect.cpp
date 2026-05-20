
#include "ModelAfterimageTrailEffect.h"

#include <ProjectNull/Actor/GhostActor/GhostActor.h>

UModelAfterimageTrailEffect::UModelAfterimageTrailEffect():
	bCanAddTrailPoint(false),
	TrailPointDataArray(TArray<AGhostActor*>()),
	TrailAddTimer(0.0f),
	TrailAddInterval(1.0f),
	TrailMaxLength(1),
	AnimPoseTime(0.0f),
	SkeletalMesh(nullptr),
	AnimationAsset(nullptr),
	GhostClass(nullptr)
{
}

void UModelAfterimageTrailEffect::Update(float DeltaTime, const FTransform& InTransform, USkeletalMesh* InSkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) {
		if (TrailAddTimer != 0.0f) { TrailAddTimer = 0.0f; }
		return;
	}

	TrailAddTimer += DeltaTime;

	if (TrailAddTimer >= TrailAddInterval) {
		AddAfterimageTrail(InTransform,InSkeletalMesh, InSnapshot);
		TrailAddTimer = 0.0f;
	}

	if (TrailPointDataArray.Num() > TrailMaxLength) {
		auto* PopData = TrailPointDataArray.Pop();
		if (!PopData) { return; }
		PopData->Destroy();
	}

}

void UModelAfterimageTrailEffect::Update(float DeltaTime, const FTransform& InTransform, USkeletalMesh* InSkeletalMesh, UAnimationAsset* InAnimation, float InPoseTime)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) {
		if (TrailAddTimer != 0.0f) { TrailAddTimer = 0.0f; }
		return;
	}

	TrailAddTimer += DeltaTime;

	if (TrailAddTimer >= TrailAddInterval) {
		AddAfterimageTrail(InTransform,InSkeletalMesh, InAnimation, InPoseTime);
		TrailAddTimer = 0.0f;
	}

	if (TrailPointDataArray.Num() > TrailMaxLength) {
		auto* PopData = TrailPointDataArray.Pop();
		if (!PopData) { return; }
		PopData->Destroy();
	}
}

void UModelAfterimageTrailEffect::AllDestroy()
{
	for (auto& Data : TrailPointDataArray) {
		Data->Destroy();
	}
}

void UModelAfterimageTrailEffect::AddAfterimageTrail(const FTransform& InTransform, USkeletalMesh* InSkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) { return; }

	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }
	Ghost->Initialize(InSkeletalMesh, InSnapshot);
	TrailPointDataArray.Insert(Ghost, 0);

	Ghost->SetActorTransform(InTransform);
}

void UModelAfterimageTrailEffect::AddAfterimageTrail(const FTransform& InTransform, USkeletalMesh* InSkeletalMesh, UAnimationAsset* InAnimation, float InPoseTime)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) { return; }

	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }
	Ghost->Initialize(InSkeletalMesh, InAnimation, InPoseTime);
	TrailPointDataArray.Insert(Ghost, 0);

	Ghost->SetActorTransform(InTransform);
}


