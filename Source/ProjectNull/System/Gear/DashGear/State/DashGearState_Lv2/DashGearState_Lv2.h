#pragma once

#include "CoreMinimal.h"

#include "../DashGearStateBase.h"
#include "../../../../../Utility/Common/GameTypes/GameTypes.h"

#include "DashGearState_Lv2.generated.h"


/// <summary>
/// ダッシュギア状態レベル2クラス
/// </summary>
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv2 final : public UDashGearStateBase
{
	GENERATED_BODY()

public:

	UDashGearState_Lv2();

public:

	void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent, class UGearBase* Gear)	override;
	void Execute(int32 CurrentGearLevel)																			override;
	void Update(float DeltaTime)																					override;


private:

	// 1回目ダッシュの閾値範囲
	UPROPERTY(EditAnywhere)
	FThresholdRange FirstDashThresholdRange;

	// 2回目ダッシュの閾値範囲
	UPROPERTY(EditAnywhere)
	FThresholdRange SecondDashThresholdRange;

	// 経過時間
	float ElapsedTime;

};
