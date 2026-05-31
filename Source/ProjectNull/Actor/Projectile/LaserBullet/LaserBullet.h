
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "LaserBullet.generated.h"

/** ルートコンポーネント */
class USceneComponent;

/** スタティックメッシュ */
class UStaticMeshComponent;

/** 飛び道具移動専用コンポーネント */
class UProjectileMovementComponent;

UCLASS()
class PROJECTNULL_API ALaserBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	ALaserBullet();

protected:
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	/** ルートコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	/** スタティックメッシュ */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	/** 飛び道具移動専用コンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
