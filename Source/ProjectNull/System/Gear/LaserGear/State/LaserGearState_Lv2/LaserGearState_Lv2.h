
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>

#include "LaserGearState_Lv2.generated.h"

/** レーザーギアの2レベル状態クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv2 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv2();
public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

private:

	UPROPERTY(EditAnywhere)
	FLaserBulletData BulletData;
};
