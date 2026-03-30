
#include "RingPulseSlashAttack.h"


URingPulseSlashAttack::URingPulseSlashAttack()
	: bRotate(false)
	, RotationSpeed(1000.0f)
	, Radius(300.0f)
	, ConeAngle(30.0f)
	, bIsActive(false)
	, CurrentAngle(0.0f)
{
	
}

void URingPulseSlashAttack::Start()
{
	bIsActive = true;
	CurrentAngle = 0.0f;
	ElapsedTime = 0.0f;
}

bool URingPulseSlashAttack::UpdateAttack(float DeltaTime)
{
	if (!bIsActive) { return false; }

	ElapsedTime += DeltaTime;

	//@‰ñ“]ˆ—
	if (bRotate)
	{
		CurrentAngle += RotationSpeed * DeltaTime;
	}

	//@I—¹”»’è
	if (ElapsedTime >= Duration)
	{
		bIsActive = false;
	}

	return true;
}

FVector URingPulseSlashAttack::CalcAttackDir(const FVector& forwardVector) const
{
	const float angle = bRotate ? CurrentAngle : 0.0f;
	return forwardVector.RotateAngleAxis(angle, FVector::UpVector);
}



