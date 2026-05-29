
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "LaserGearStateBase.generated.h"


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
