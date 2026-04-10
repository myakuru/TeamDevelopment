
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponStateBase.generated.h"

/// <summary>
//@•‚—V•Ší‚Ìó‘ÔŠî’êƒNƒ‰ƒX
/// </summary>
UCLASS()
class PROJECTNULL_API UFloatingWeaponStateBase : public UObject
{
	GENERATED_BODY()

public:

	UFloatingWeaponStateBase();

public:

	virtual void Update(float DeltaTime) { return; }

};
