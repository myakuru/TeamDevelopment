
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostActor.generated.h"

/** ゴーストアクタークラス
* アクターオブジェクトの残像表現 */
UCLASS()
class PROJECTNULL_API AGhostActor final : public AActor
{
	GENERATED_BODY()
public:	
	AGhostActor();
protected:

	void BeginPlay() override;
public:	

	void Tick(float DeltaTime) override;

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh 残像表現するスケルタルメッシュ
	 * @param Animation アニメーション
	 * @param PoseTime 固定するアニメーションを地点時間
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					TObjectPtr<UMaterialInterface> GhostMaterial,
					float PoseTime);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/*UPROPERTY(EditAnywhere, Category = "Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;*/

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	float LifeTime = 0.5f;
	float CurrentTime = 0.0f;
};
