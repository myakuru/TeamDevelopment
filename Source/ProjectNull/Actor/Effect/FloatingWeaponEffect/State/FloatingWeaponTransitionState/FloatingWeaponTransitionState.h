
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponTransitionState.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponTransitionState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
public:
	UFloatingWeaponTransitionState();
public:
	void Update(AActor* OwnerActor, float DeltaTime)override;

};
