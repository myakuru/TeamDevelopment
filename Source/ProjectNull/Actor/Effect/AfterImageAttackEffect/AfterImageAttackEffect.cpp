
#include "AfterImageAttackEffect.h"

#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Effect/ModelAfterimageTrailEffect/ModelAfterimageTrailEffect.h>

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
		if (Time > Data.MoveTime) { 
			if (!Data.ModelAfterimageTrailEffect)
			{
				continue;
			}

			Data.ModelAfterimageTrailEffect->SetCanAddTrailPoint(false);
			continue; 
		}


		if (!Data.bSpawn) {
			Data.bSpawn = true;

			if (!Data.ModelAfterimageTrailEffect)
			{
				continue;
			}
		
			Data.ModelAfterimageTrailEffect->SetCanAddTrailPoint(true);
		}

		if (Data.ModelAfterimageTrailEffect) {

			const FVector PlayerLocation = StartTransfrom.GetLocation();
			const FMatrix PlayerMatrix = StartTransfrom.ToMatrixWithScale();

			const FVector Forward = PlayerMatrix.GetUnitAxis(EAxis::X);
			const FVector Right = PlayerMatrix.GetUnitAxis(EAxis::Y);
			const FVector Up = PlayerMatrix.GetUnitAxis(EAxis::Z);
			const FVector Offset = Data.CalcLocationOffset(Time / Data.MoveTime);
			UE_LOG(LogTemp, Warning, TEXT("hi time %.2f"), Offset.X);

			const FVector ResultLocation = PlayerLocation
				+ Forward * Offset.X
				+ Right * Offset.Y
				+ Up * Offset.Z;
			const FVector MoveDir = Data.CalcMoveDir();
			Data.Transform.SetLocation(ResultLocation);
			Data.Transform.SetRotation((MoveDir.Rotation() + Data.RotationOffset).Quaternion());
			Data.Transform.SetScale3D(Data.Scale);

			Data.ModelAfterimageTrailEffect->Update(DeltaTime, Data.Transform,SkeletalMesh,AnimationAsset,Data.PoseTime);
		}

	}
}

float UAfterImageAttackEffect::GetMaxTime()
{
	float MaxTime = 0.0f;

	for (auto& Data : AfterImageDataArray) {
		if (Data.TimeThreshold + Data.MoveTime > MaxTime)
		{
			MaxTime = Data.TimeThreshold + Data.MoveTime;
		}
	}

	return MaxTime;
}
