#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CapsuleCollision.generated.h"

/** カプセル状の当たり判定コンポーネント */
class UCapsuleComponent;

UCLASS()
class PROJECTNULL_API ACapsuleCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACapsuleCollision();

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
	TObjectPtr<UCapsuleComponent> GetCapsuleComponent()const
	{
		return CapsuleComponent;
	}
	/* End Getters~ */

private:

	UPROPERTY(EditAnywhere)
	TObjectPtr<UCapsuleComponent> CapsuleComponent;
};
