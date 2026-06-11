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
	void DetachRootComponent();

	/* ~Begin Setters */
	/**
	 * @brief 自身のルートを持ち主のルートへアタッチする
	 * @param InOwnerRoot 持ち主のルート
	 */
	void AttachOwnerRoot(const TObjectPtr<USceneComponent>& InOwnerRoot)
	{
		RootComponent->SetupAttachment(InOwnerRoot);
	}
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
