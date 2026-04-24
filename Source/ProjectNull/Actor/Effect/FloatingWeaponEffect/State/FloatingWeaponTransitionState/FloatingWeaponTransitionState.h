
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponTransitionState.generated.h"

/// <summary>
/// 浮遊武器の遷移状態クラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponTransitionState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
public:
	UFloatingWeaponTransitionState();
public:
	void Start(EFloatingWeaponState SetNextState)	override;
	void Update(float DeltaTime)					override;
private:

	
	void UpdateTransformLerp(float DeltaTime);



	// 遷移先浮遊武器状態
	EFloatingWeaponState NextState;

	// 現在の状態開始座標オフセット
	FVector StartLocationOffset;

	// 現在の状態開始回転オフセット
	FQuat StartRotationOffset;

	// 遷移先の目指すべきTransform情報
	FTransform TargetTransform;
};
