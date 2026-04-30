
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponTransitionState.generated.h"

/// <summary>
/// ���V����̑J�ڏ�ԃN���X
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

	
	void UpdateTransformOffsetLerp(float DeltaTime);

	void TryChangeToStandState();

	void TryChangeToAttackState();

	EFloatingWeaponState NextState;

	

	FTransform StartTransform;
	FTransform TargetTransform;
};
