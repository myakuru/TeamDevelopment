
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include "ReflectiveLaser.generated.h"

/** 反射機能を持つレーザー(弾タイプ)クラス */
UCLASS()
class PROJECTNULL_API AReflectiveLaser final : public AProjectileBase
{
	GENERATED_BODY()
public:
	AReflectiveLaser();
	AReflectiveLaser(int32 InReflectionCount);
public:

	void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

private:

	/** レーザーを反射する */
	void ReflectLaserBullet();

	/** 反射インターバル管理タイマー */
	FTimerHandle ReflectionIntervalTimerHandle;

	/** 反射までのインターバル */
	UPROPERTY(EditAnywhere)
	float ReflectionInterval;

	/** 反射回数 */
	UPROPERTY(EditAnywhere)
	int32 ReflectionCount;
};
