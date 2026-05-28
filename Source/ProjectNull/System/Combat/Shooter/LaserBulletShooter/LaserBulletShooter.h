
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "LaserBulletShooter.generated.h"


/** 発射物の中間基底クラス */
class AProjectileBase;


/** レーザー(弾タイプ)の発射制御を行うクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserBulletShooter final : public UObject
{
	GENERATED_BODY()
public:
	ULaserBulletShooter();
public:

	inline void InitVelocityArray(const FVector& Velocity)
	{
		StartVelocityArray.Init(Velocity, Num);
	}


	inline void SetOwner(AActor* InOwner) { Owner = InOwner; }

	void ShotTargetedLaserBullets(const FVector& InSpawnLocation);

	void Update(float DeltaTime);

private:

	void ShotLaserBullet(const FVector& InSpawnLocation);

	UPROPERTY()
	TObjectPtr<AActor> Owner;

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
