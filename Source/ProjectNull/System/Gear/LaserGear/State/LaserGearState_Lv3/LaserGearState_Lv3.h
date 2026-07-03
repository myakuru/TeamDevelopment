
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>

#include "LaserGearState_Lv3.generated.h"

class UCrossLaserAttack;

/** レーザーギアの3レベル状態クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv3 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv3();
public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	inline const int32 GetGearLevelIndex() const	override { return kLv3Index; }


private:

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UCrossLaserAttack> CrossLaserAttack;
};
