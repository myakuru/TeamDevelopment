#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>
#include <ProjectNull/System/Gear/DashGear/State/DashGearStateBase.h>

#include "DashGearState_Lv2.generated.h"

/** ダッシュギアのレベル2状態クラス */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv2 final : public UDashGearStateBase
{
	GENERATED_BODY()

public:

	UDashGearState_Lv2();

public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;


private:

	UPROPERTY(EditAnywhere)
	FThresholdRange FirstDashThresholdRange;

	UPROPERTY(EditAnywhere)
	FThresholdRange SecondDashThresholdRange;

	float ElapsedTime;

};
