
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponAttackState.generated.h"

// ���V����U��
class UFloatingWeaponAttack;

/// <summary>
// ���V����̍U����ԃN���X
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttackState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()

public:
	UFloatingWeaponAttackState();
public:

	void Update(float DeltaTime)override;

	FTransform CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle);
};
