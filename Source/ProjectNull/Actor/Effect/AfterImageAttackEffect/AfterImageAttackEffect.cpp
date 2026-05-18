
#include "AfterImageAttackEffect.h"

#include <ProjectNull/Actor/GhostActor/GhostActor.h>

UAfterImageAttackEffect::UAfterImageAttackEffect()
{

}

void UAfterImageAttackEffect::Initialize()
{

}

void UAfterImageAttackEffect::Start()
{

}

void UAfterImageAttackEffect::Update(float DeltaTime, float ElapsedTime, const FTransform& PlayerTransform)
{

	for (auto& Data : AfterImageDataArray) {

		if (Data.TimeThreshold > ElapsedTime) { continue; }
		// 経過時間から閾値を引いてこの区間だけの時間を算出する
		const float Time = ElapsedTime - Data.TimeThreshold;
		if (Time > Data.Time) { continue; }

		AGhostActor* Ghost = nullptr;

		if (!Data.bSpawn) {
			Ghost = GetWorld()->SpawnActor<AGhostActor>(GhostClass);

			if (Ghost) {

				Ghost->SetActorTransform(Data.Transform);
				Ghost->Initialize(SkeletalMesh,AnimationAsset,Data.PoseTime,Data.LifeTime, Data.OpacityDecayRate);
				Data.GhostActor = Ghost;
			}
			Data.bSpawn = true;
		}

		if (Data.GhostActor) {

			const FVector PlayerLocation = PlayerTransform.GetLocation();
			const FMatrix PlayerMatrix = PlayerTransform.ToMatrixWithScale();

			const FVector Forward = PlayerMatrix.GetUnitAxis(EAxis::X);
			const FVector Right = PlayerMatrix.GetUnitAxis(EAxis::Y);
			const FVector Up = PlayerMatrix.GetUnitAxis(EAxis::Z);
			const FVector Offset = Data.CalcLocationOffset(Time / Data.Time);

			const FVector ResultLocation = PlayerLocation
				+ Forward * Offset.X
				+ Right * Offset.Y
				+ Up * Offset.Z;
			UE_LOG(LogTemp, Warning, TEXT("hi time %.2f"), Time / Data.Time);
			const FVector MoveDir = (ResultLocation - Data.Transform.GetLocation()).GetSafeNormal();
			Data.Transform.SetLocation(ResultLocation);
			Data.Transform.SetRotation((MoveDir.Rotation() + Data.RotationOffset).Quaternion());
			Data.Transform.SetScale3D(Data.Scale);
			Data.GhostActor->SetActorTransform(Data.Transform);
		}

	}
}

float UAfterImageAttackEffect::GetMaxTime()
{
	float MaxTime = 0.0f;

	for (auto& Data : AfterImageDataArray) {
		if (Data.TimeThreshold + Data.Time > MaxTime)
		{
			MaxTime = Data.TimeThreshold + Data.Time;
		}
	}

	return MaxTime;
}
