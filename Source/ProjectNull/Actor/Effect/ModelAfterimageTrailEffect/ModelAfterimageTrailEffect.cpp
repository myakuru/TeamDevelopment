
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

void UModelAfterimageTrailEffect::Update(float DeltaTime)
{ 
	if (!bCanAddTrailPoint) {

		if (TrailPointDataArray.Num() != 0) {
			AllDestroy();
		}

		if(TrailAddTimer != 0.0f) { TrailAddTimer = 0.0f; }
		return;
	}
	
	TrailAddTimer += DeltaTime;

	if (TrailAddTimer >= TrailAddInterval) {
		AddAfterimageTrail();
		TrailAddTimer = 0.0f;
	}

	if (TrailPointDataArray.Num() > TrailMaxLength) {
		auto* PopData = TrailPointDataArray.Pop();
		if (!PopData) { return; }
		PopData->Destroy();
	}

	for (auto& Data : TrailPointDataArray) {
		Data->Update(DeltaTime);
	}
}

void UModelAfterimageTrailEffect::Update(float DeltaTime, USkeletalMesh* InSkeletalMesh, UAnimationAsset* InAnimationAsset, float InAnimPoseTime)
{
	if (!InSkeletalMesh || !InAnimationAsset) { return; }
	if (!bCanAddTrailPoint) {
		if (TrailAddTimer != 0.0f) { TrailAddTimer = 0.0f; }
		return;
	}

	TrailAddTimer += DeltaTime;

	if (TrailAddTimer >= TrailAddInterval) {
		AddAfterimageTrail(InSkeletalMesh,InAnimationAsset,InAnimPoseTime);
		TrailAddTimer = 0.0f;
	}

	if (TrailPointDataArray.Num() > TrailMaxLength) {
		auto* PopData = TrailPointDataArray.Pop();
		if (!PopData) { return; }
		PopData->Destroy();
	}

	for (auto& Data : TrailPointDataArray) {
		Data->Update(DeltaTime);
	}
}

void UModelAfterimageTrailEffect::Update(float DeltaTime, USkeletalMesh* InSkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) {
		if (TrailAddTimer != 0.0f) { TrailAddTimer = 0.0f; }
		return;
	}

	TrailAddTimer += DeltaTime;

	if (TrailAddTimer >= TrailAddInterval) {
		AddAfterimageTrail(InSkeletalMesh,InSnapshot);
		TrailAddTimer = 0.0f;
	}

	if (TrailPointDataArray.Num() > TrailMaxLength) {
		auto* PopData = TrailPointDataArray.Pop();
		if (!PopData) { return; }
		PopData->Destroy();
	}

	for (auto& Data : TrailPointDataArray) {
		Data->Update(DeltaTime);
	}
}

void UModelAfterimageTrailEffect::AllDestroy()
{
	for (auto& Data : TrailPointDataArray) {
		Data->Destroy();
	}
}

void UModelAfterimageTrailEffect::AddAfterimageTrail()
{
	if (!bCanAddTrailPoint) { return; }

	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }
	Ghost->Initialize(SkeletalMesh, AnimationAsset, AnimPoseTime);

	if (!OwnerActor) { return; }

	Ghost->SetActorTransform(OwnerActor->GetActorTransform());

	TrailPointDataArray.Insert(Ghost, 0);
}

void UModelAfterimageTrailEffect::AddAfterimageTrail(USkeletalMesh* InSkeletalMesh, UAnimationAsset* InAnimationAsset, float InAnimPoseTime)
{
	if (!InSkeletalMesh || !InAnimationAsset) { return; }
	if (!bCanAddTrailPoint) { return; }

	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }
	Ghost->Initialize(InSkeletalMesh, InAnimationAsset, InAnimPoseTime);

	if (!OwnerActor) { return; }

	Ghost->SetActorTransform(OwnerActor->GetActorTransform());

	TrailPointDataArray.Insert(Ghost, 0);
}

void UModelAfterimageTrailEffect::AddAfterimageTrail(USkeletalMesh* InSkeletalMesh, const FPoseSnapshot& InSnapshot)
{
	if (!InSkeletalMesh) { return; }
	if (!bCanAddTrailPoint) { return; }

	auto* Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);
	if (!Ghost) { return; }
	Ghost->Initialize(InSkeletalMesh,InSnapshot);

	if (!OwnerActor) { return; }

	Ghost->SetActorTransform(OwnerActor->GetActorTransform());

	TrailPointDataArray.Insert(Ghost, 0);
}

