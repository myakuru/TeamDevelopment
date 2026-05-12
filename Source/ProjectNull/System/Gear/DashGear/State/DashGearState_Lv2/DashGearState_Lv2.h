#pragma once

#include "CoreMinimal.h"

#include "../DashGearStateBase.h"
#include "../../../../../Utility/Common/GameTypes/GameTypes.h"

#include "DashGearState_Lv2.generated.h"

/** ダッシュギアのレベル2状態クラス */
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

	// 1��ڃ_�b�V����臒l�͈�
	UPROPERTY(EditAnywhere)
	FThresholdRange FirstDashThresholdRange;

	// 2��ڃ_�b�V����臒l�͈�
	UPROPERTY(EditAnywhere)
	FThresholdRange SecondDashThresholdRange;

	// �o�ߎ���
	float ElapsedTime;

};
