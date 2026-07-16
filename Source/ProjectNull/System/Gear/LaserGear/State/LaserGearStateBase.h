
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "LaserGearStateBase.generated.h"

namespace SEIndex {
	inline constexpr int32 LaserGunSESoundIndex		= 0;
	inline constexpr int32 CrossLaserSESoundIndex	= 1;
	inline constexpr int32 LaserChargeSESoundIndex	= 2;
	inline constexpr int32 BigLaserSESoundIndex		= 3;
}

/** レーザー(弾タイプ)の発射制御を行うクラス */
class ULaserBulletShooter;

/** レーザーギアの状態中間基底クラス */
UCLASS()
class PROJECTNULL_API ULaserGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearStateBase();

private:

protected:
	
	/** デバッグライン描画するかどうか */
	UPROPERTY(EditAnywhere)
	bool bDrawDebugLine;
};
