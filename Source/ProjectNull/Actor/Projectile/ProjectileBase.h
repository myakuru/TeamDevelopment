
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "ProjectileBase.generated.h"

class UProjectileMovementComponent;

UCLASS()
class PROJECTNULL_API AProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectileBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:

	/** ルートコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	/** 残像表示用スケルタルメッシュ */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** 飛び道具移動専用コンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditAnywhere)
	float Speed;

	
};
