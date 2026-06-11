
#include "GroundUtility.h"

UGroundUtility::UGroundUtility()
{
}

FQuat UGroundUtility::MakeRotationFromGroundNormal(
	const FTransform& InTransform,
	const FVector& InGroundNormal)
{
	const FMatrix& WorldMatrix	= InTransform.ToMatrixWithScale();
	const FVector& Forward		= WorldMatrix.GetUnitAxis(EAxis::X);
	const FVector& Right		= WorldMatrix.GetUnitAxis(EAxis::Y);

	FVector NewForward = FVector::VectorPlaneProject(
			Forward,
			InGroundNormal);

	if (NewForward.IsNearlyZero())
	{
		NewForward = FVector::CrossProduct(
			Right,
			InGroundNormal);
	}

	NewForward.Normalize();

    return FRotationMatrix::MakeFromZX(
		InGroundNormal,
		Forward)
		.ToQuat();
}
