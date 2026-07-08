
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponAttackState.generated.h"

class UFloatingWeaponAttack;

UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttackState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
public:
	UFloatingWeaponAttackState();

	void Start()				override;
	void Update(float DeltaTime)override;

	FTransform CalcAttackStateTransformOffset(
		UFloatingWeaponAttack* OwnerAttack,
		float RotatorOffsetAngle);
};
