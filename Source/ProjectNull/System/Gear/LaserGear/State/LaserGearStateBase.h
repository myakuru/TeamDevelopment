
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "LaserGearStateBase.generated.h"

/** 発射物の中間基底クラス */
class AProjectileBase;


USTRUCT()
struct FLaserBulletData
{
	GENERATED_BODY()
public:
	FLaserBulletData()
	{
	}
public:

	/** レーザー(弾タイプ)クラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> Class;

	/** レーザー(弾タイプ)を発射する数 */
	UPROPERTY(EditAnywhere)
	int32 Num;

	/** レーザー(弾タイプ)の速度 */
	UPROPERTY(EditAnywhere)
	float Speed;
};


/** レーザーギアの状態中間基底クラス */
UCLASS()
class PROJECTNULL_API ULaserGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearStateBase();

private:

protected:
	
	
	UPROPERTY(EditAnywhere)
	bool bDrawDebugLine;
};
