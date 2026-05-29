
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "LaserBulletShooter.generated.h"


/** 発射物の中間基底クラス */
class AProjectileBase;

/** プレイヤーの中間基底クラス */
class APlayerBase;


/** レーザー(弾タイプ)の発射制御を行うクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserBulletShooter final : public UObject
{
	GENERATED_BODY()
public:
	ULaserBulletShooter();
public:

	void Initialize(AActor* InOwner);

	void ShotTargetedLaserBullets(const FVector& InStartLocation);


	void Reset();

	/** Getter */
	inline float GetTargetableDistSq() const { return TargetableDistSq; }

private:


	void ShotLaserBulletAndIncrementCount();

	/**
	 * @brief レーザー(弾タイプ)の発射処理
	 */
	void ShotLaserBullet();


	inline void InitVelocityArray(const FVector& Velocity)
	{
		StartVelocityArray.Init(Velocity, BulletNum);
	}


	/** 持ち主のクラス */
	UPROPERTY()
	TObjectPtr<AActor> Owner;

	/** プレイヤーの中間基底クラス */
	UPROPERTY()
	TObjectPtr<APlayerBase> Player;

	/** レーザー(弾タイプ)クラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectileBase> ProjectileClass;

	/** レーザー(弾タイプ)が発射される位置 */
	FVector StartLocation;

	/** 各レーザー開始加速度配列 */
	TArray<FVector> StartVelocityArray;

	/** 敵をターゲットできる距離Sq */
	UPROPERTY(EditAnywhere)
	float TargetableDistSq;

	/** レーザー(弾タイプ)を発射する数 */
	UPROPERTY(EditAnywhere)
	int32 BulletNum;

	/** レーザー(弾タイプ)の速度 */
	UPROPERTY(EditAnywhere)
	float BulletSpeed;

	/** 発射間隔 */
	UPROPERTY(EditAnywhere)
	float ShotInterval;

	/** 発射した弾数 */
	int32 ShotCount;

	/** 発射間隔管理用 */
	FTimerHandle ShotIntervalTimerHandle;

};
