#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/DashGear/State/DashGearStateBase.h>

#include "DashGearState_Lv3.generated.h"

/** ダッシュギアのレベル3状態クラス */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv3 final : public UDashGearStateBase
{
	GENERATED_BODY()
public:
	UDashGearState_Lv3();
};
