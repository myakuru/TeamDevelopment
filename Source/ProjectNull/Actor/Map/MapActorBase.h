#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapActorBase.generated.h"

/**
 * マップに配置するアクターの基底クラス
 * 基本プレイヤーイベントがあるオブジェクトのみ
 */

 //マップアクターのパラメータ
USTRUCT(BlueprintType)
struct FActorParams
{
	GENERATED_BODY()
public:

	/** 当たる距離 */
	UPROPERTY(EditAnywhere)
	float HitDistance = 100.0f;
};

UCLASS()
class PROJECTNULL_API AMapActorBase : public AActor
{
	GENERATED_BODY()

public:	

	AMapActorBase();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;

	/** プレイヤーとの距離当たり判定 */
	virtual bool OnHitDistancePlayer();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
	FActorParams ActorParams;
};
