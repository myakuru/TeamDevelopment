
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponAttackState.generated.h"

/** オフセット計算結果返り値用構造体 */
USTRUCT(BlueprintType)
struct FCalcResultOffset
{
	GENERATED_BODY()

public:

	FCalcResultOffset() :
		Transform(FTransform()),
		YawOffset(0.0f)
	{
	}

	/** Transformオフセット */
	FTransform Transform;

	/** Yawオフセット */
	float YawOffset;
};

// 浮遊武器攻撃
class UFloatingWeaponAttack;

/// <summary>
// 浮遊武器の攻撃状態クラス
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
	/// 攻撃状態のTransformオフセットの計算
	/// </summary>
	/// <param name="OwnerAttack"></param>
	/// <param name="RotatorOffsetAngle"></param>
	/// <returns></returns>
	FCalcResultOffset CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle);
};
