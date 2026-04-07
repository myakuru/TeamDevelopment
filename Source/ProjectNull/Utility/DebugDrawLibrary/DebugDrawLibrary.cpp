
#include "DebugDrawLibrary.h"


void UDebugDrawLibrary::DrawDebugFan(UWorld* World, FVector Center, FVector Forward, float Radius, float AngleDeg, int32 Segments)
{
	float HalfAngle = AngleDeg * 0.5f;

	//　左端と右端
	FVector LeftDir = Forward.RotateAngleAxis(-HalfAngle, FVector::UpVector);
	FVector RightDir = Forward.RotateAngleAxis(HalfAngle, FVector::UpVector);

	DrawDebugLine(World, Center, Center + LeftDir * Radius, FColor::Green, false, 0.1f, 0, 2);
	DrawDebugLine(World, Center, Center + RightDir * Radius, FColor::Green, false, 0.1f, 0, 2);

	//　扇のカーブ部分
	float Step = AngleDeg / Segments;

	FVector PrevPoint = Center + LeftDir * Radius;

	for (int i = 1; i <= Segments; i++)
	{
		float CurrentAngle = -HalfAngle + Step * i;
		FVector Dir = Forward.RotateAngleAxis(CurrentAngle, FVector::UpVector);

		FVector NextPoint = Center + Dir * Radius;

		DrawDebugLine(World, PrevPoint, NextPoint, FColor::Green, false, 0.1f, 0, 2);

		PrevPoint = NextPoint;
	}
}
