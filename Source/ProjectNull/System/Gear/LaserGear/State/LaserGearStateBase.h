
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "LaserGearStateBase.generated.h"

/** 発射物の中間基底クラス */
class AProjectileBase;

/** レーザーギアの状態中間基底クラス */
UCLASS()
class PROJECTNULL_API ULaserGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearStateBase();
};
