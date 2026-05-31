
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include "ReflectiveLaser.generated.h"

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

	virtual void Tick(float DeltaTime) override;

private:

	void HandleCollision(AActor* OtherActor) override;

	/** レーザーを反射する */
	void ReflectLaserBullet(const FVector& FindLocation);

	void UpdateSphereCollision();

	/** 反射インターバル管理タイマー */
	FTimerHandle ReflectionIntervalTimerHandle;

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

	bool bEnable;


};
