#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BoxCollision.generated.h"

/** Box状の当たり判定コンポーネント */
class UBoxComponent;

UCLASS()
class PROJECTNULL_API ABoxCollision : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABoxCollision();

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
	/**　Box状当たり判定コンポーネントの取得 */
	TObjectPtr<UBoxComponent> GetBoxComponent()const
	{
		return BoxComponent;
	}
	/* End Getters~ */

private:

	/**
	 * @brief 球状の当たり判定コンポーネント
	 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;


};
