
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
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

	void Start(EFloatingWeaponState SetNextState) override;
	void Update(AActor* OwnerActor, float DeltaTime)override;

private:

	void UpdateAttakTransition(AActor* OwnerActor, float DeltaTime);

	void UpdateStandTransition(AActor* OwnerActor, float DeltaTime);

	EFloatingWeaponState NextState;
};
