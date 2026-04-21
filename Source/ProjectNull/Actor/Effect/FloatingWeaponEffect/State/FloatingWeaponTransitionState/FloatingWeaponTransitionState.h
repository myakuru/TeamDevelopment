
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "../../../FloatingWeaponEffect/FloatingWeaponEffect.h"
#include "FloatingWeaponTransitionState.generated.h"

/// <summary>
/// •‚—V•Ší‚Ì‘JˆÚó‘ÔƒNƒ‰ƒX
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponTransitionState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
public:
	UFloatingWeaponTransitionState();

public:

	void Start(EFloatingWeaponState SetNextState);
	void Update(AActor* OwnerActor, float DeltaTime)override;
	void UpdateTransitionTime(float DeltaTime);


	bool IsFinishedTransitionState() const
	{
		return TransitionTime <= 0.0f;
	}

	float TransitionTime;
	EFloatingWeaponState NextState;
};
