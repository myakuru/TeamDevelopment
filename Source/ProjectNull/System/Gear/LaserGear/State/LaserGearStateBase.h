
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "LaserGearStateBase.generated.h"

/** 発射物の中間基底クラス */
class AProjectileBase;

/** レーザー(弾タイプ)の構造体データ */
USTRUCT()
struct FLaserBulletData
{
	GENERATED_BODY()
public:
	FLaserBulletData():
		Class(nullptr),
		TargetableDistSq(0.0f),
		Num(1),
		Speed(0.f),
		ShotInterval(0.f),
		ShotCount(1)
	{
	}

public:

	inline void InitVelocityArray(const FVector& Velocity)
	{
		StartVelocityArray.Init(Velocity,Num);
	}

	/** レーザー(弾タイプ)クラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> Class;

	/** 各レーザー開始加速度配列 */
	TArray<FVector> StartVelocityArray;

	/** 敵をターゲットできる距離Sq */
	UPROPERTY(EditAnywhere)
	float TargetableDistSq;

	/** レーザー(弾タイプ)を発射する数 */
	UPROPERTY(EditAnywhere)
	int32 Num;

	/** レーザー(弾タイプ)の速度 */
	UPROPERTY(EditAnywhere)
	float Speed;

	/** 発射間隔 */
	UPROPERTY(EditAnywhere)
	float ShotInterval;

	/** 発射した弾数 */
	int32 ShotCount;

	/** 発射間隔管理用 */
	FTimerHandle ShotIntervalTimerHandle;

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
	
	void ShotTargetedLaserBullets(FLaserBulletData& InBulletData);

	void ShotLaserBullet(
		const FLaserBulletData& InBulletData,
		const FVector& InSpawnLocation);
	// これを構造体におく
	void Update

	/** デバッグライン描画するかどうか */
	UPROPERTY(EditAnywhere)
	bool bDrawDebugLine;
};
