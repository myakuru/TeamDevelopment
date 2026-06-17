#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SphereCollision.generated.h"

/** 球状の当たり判定コンポーネント */
class USphereComponent;

UCLASS()
class PROJECTNULL_API ASphereCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphereCollision();

public:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/**
	 * @brief ルートの親子関係を解除
	 */
	void DetachRootComponent()
	{
		if (!RootComponent) { return; }

		RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
	}

	/**
	 * @brief コリジョンチャンネルとそれに対するレスポンスをセット
	 * @param CollisionChannel	判定したいチャンネル(PawnやEnemy..)
	 * @param CollisionResponse それに対するレスポンス
	 */
	void SetCollisionResponseToChannnel(
		const ECollisionChannel		InCollisionChannel,
		const ECollisionResponse	InCollisionResponse);

	/**
	 * @brief コリジョン判定の有効無効の切り替え
	 * @param InEnabled NoCollisionで無効
	 */
	void SetCollisionEnabled(const ECollisionEnabled::Type InEnabled);

	/**
	 * @brief オフセット用のTransformに変更する
	 * @param InOffsetTransform オフセット用Transform
	 */
	void SetRelativeTransform(const FTransform& InOffsetTransform);
	/* End Setters~ */

	/* ~Begin Getters */
	/**　球状当たり判定コンポーネントの取得 */
	TObjectPtr<USphereComponent> GetSphereComponent()const
	{
		return SphereComponent;
	}
	/* End Getters~ */

private:

	/**
	 * @brief 球状の当たり判定コンポーネント
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

};
