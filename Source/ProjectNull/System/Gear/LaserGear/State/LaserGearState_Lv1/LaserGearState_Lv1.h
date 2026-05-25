
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>

#include "LaserGearState_Lv1.generated.h"

/** レーザーギアの1レベル状態クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv1 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv1();
public:
};
