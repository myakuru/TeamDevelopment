
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponAttackState.generated.h"

// •‚—V•ŠíUŒ‚
class UFloatingWeaponAttack;

/// <summary>
// •‚—V•Ší‚ÌUŒ‚ó‘ÔƒNƒ‰ƒX
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttackState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()

public:
	UFloatingWeaponAttackState();
public:
	void Update(float DeltaTime)override;

	/// <summary>
	/// UŒ‚ó‘Ô‚ÌTransformƒIƒtƒZƒbƒg‚ÌŒvZ
	/// </summary>
	/// <param name="OwnerAttack"></param>
	/// <param name="RotatorOffsetAngle"></param>
	/// <returns></returns>
	FTransform CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle);
};
