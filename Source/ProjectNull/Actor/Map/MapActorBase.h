#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
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

	virtual void OnConstruction(const FTransform& Transform) override;

	/** プレイヤーとの距離当たり判定 */
	virtual float DistanceFromPlayer();

	/** 何らかに当たった時のリアクション
	    各々のクラスでオーバーライド*/
	UFUNCTION()
	virtual void HitReaction(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	){ }

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	/** メッシュ */
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	/** トリガーボリューム */
	UPROPERTY(VisibleAnywhere)
	USphereComponent* Trigger;

	/** アクターパラメータ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params")
	FActorParams ActorParams;
};
