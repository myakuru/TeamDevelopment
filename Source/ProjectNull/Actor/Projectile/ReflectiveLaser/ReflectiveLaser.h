
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include "ReflectiveLaser.generated.h"

/** 敵の中間基底クラス */
class AEnemyBase;

/** 反射機能を持つレーザー(弾タイプ)クラス */
UCLASS(Blueprintable)
class PROJECTNULL_API AReflectiveLaser final : public AProjectileBase
{
	GENERATED_BODY()
public:
	AReflectiveLaser();
	AReflectiveLaser(int32 InReflectionCount);
public:

private:

	void HandleCollision(AActor* OtherActor) override;

	/** レーザーを反射する */
	void ReflectLaserBullet(const FVector& FindLocation);

	/** レーザー(弾タイプ)の速度 */
	UPROPERTY(EditAnywhere)
	float BulletSpeed;

	/** 反射までのインターバル */
	UPROPERTY(EditAnywhere)
	float ReflectionInterval;

	/** 反射地点からの対象探索距離Sq */
	UPROPERTY(EditAnywhere)
	float FindDistSq;

	/** 反射回数 */
	UPROPERTY(EditAnywhere)
	int32 ReflectionCount;

	/** 既に反射した敵 */
	UPROPERTY()
	TSet<TWeakObjectPtr<AEnemyBase>> ReflectedEnemies;

	/** 反射インターバル管理タイマー */
	FTimerHandle ReflectionIntervalTimerHandle;
};
