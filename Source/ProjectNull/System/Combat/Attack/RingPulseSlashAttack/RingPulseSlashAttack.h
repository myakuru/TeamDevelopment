#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "RingPulseSlashAttack.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API URingPulseSlashAttack final : public UAttackBase
{
	GENERATED_BODY()
	
public:

	URingPulseSlashAttack();

public:

	void Start();

	bool UpdateAttack(float DeltaTime);

	bool CanDeactivate();

	bool IsActiveFirstFrame();

	void UpdatePrevActiveFlg();

	FVector CalcAttackDir(const FVector& forwardVector) const;

	inline float GetRadiusSquared() const { return Radius * Radius; }

	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

	UPROPERTY(EditAnywhere)
	float Duration;

	float ElapsedTime;

	UPROPERTY(EditAnywhere)
	bool bRotate;

	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere)
	float Radius;

	UPROPERTY(EditAnywhere)
	float ConeAngle;

	bool bIsActive;

	bool bPrevActive;

	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	UPROPERTY(EditAnywhere)
	float StartAngle;

	UPROPERTY(EditAnywhere)
	float KnockbackPower;

};
