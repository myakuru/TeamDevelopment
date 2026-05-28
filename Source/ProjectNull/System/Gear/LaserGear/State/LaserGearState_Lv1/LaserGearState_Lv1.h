
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

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	/** Getter */
	inline const int32 GetGearLevelIndex() const override { return kLv1Index; }

private:


	UPROPERTY(EditAnywhere)
	FLaserBulletData BulletData;

};
