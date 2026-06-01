
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


	virtual void HandleCollision(AActor* OtherActor);

	/** 持ち主のアクタークラス */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

public:	
	virtual void Tick(float DeltaTime) override;

	/** Getter */
	inline USphereComponent*				GetSphereCollision() const		{ return SphereCollision; }
	inline UStaticMeshComponent*			GetStaticMeshComponent() const	{ return StaticMesh; }
	inline UProjectileMovementComponent*	GetProjectileMovement() const	{ return ProjectileMovement; }

	/** Setter */
	inline void SetOwnerActor(AActor* InOwnerActor) { OwnerActor = InOwnerActor; }

private:

	UFUNCTION()
	void OnCollisionOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnProjectileStop(const FHitResult& Hit);

	
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
