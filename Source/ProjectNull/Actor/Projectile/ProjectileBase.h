
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

/** ルートコンポーネント */
class USceneComponent;

/** スフィアコリジョンコンポーネント */
class USphereComponent;

/** スタティックメッシュ */
class UStaticMeshComponent;

/** 飛び道具移動専用コンポーネント */
class UProjectileMovementComponent;

/** 発射物の中間基底クラス */
UCLASS(Blueprintable)
class PROJECTNULL_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	inline UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

private:

	/** ルートコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	/** スフィアコリジョンコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereCollision;

	/** スタティックメッシュ */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	/** 飛び道具移動専用コンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;
};
