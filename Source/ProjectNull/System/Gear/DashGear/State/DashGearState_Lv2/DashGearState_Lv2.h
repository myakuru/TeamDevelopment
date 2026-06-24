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
	void End()								override;

private:

	void InitializeGearDuration();

	void EndFirstDash();
	void StartSecondDash();

	
	UPROPERTY(EditAnywhere)
	float FirstDashDuration;

	UPROPERTY(EditAnywhere)
	float FirstDashToDelay;

	UPROPERTY(EditAnywhere)
	float SecondDashDuration;

	bool bShouldDash;

	FTimerHandle FirstDashTimerHandle;
	FTimerHandle FirstDashToDelayTimerHandle;

};
