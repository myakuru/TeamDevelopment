
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GhostActor.generated.h"

/** ゴーストアクタークラス
* アクターオブジェクトの残像表現クラス */
UCLASS()
class PROJECTNULL_API AGhostActor final : public AActor
{
	GENERATED_BODY()
public:	
	AGhostActor();
protected:

	void BeginPlay() override;
public:	


	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					float InAnimPoseTime);

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh 残像表現するスケルタルメッシュ
	 * @param Animation アニメーション
	 * @param PoseTime 固定するアニメーションを地点時間
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					float PoseTime,
					float InLifeTime,
					float InOpacityDecayRate);

	void Update(float DeltaTime);

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	/** 透過度のソート優先度 */
	UPROPERTY(EditAnywhere)
	int32 TranslucencySortPriority;

	/** 描画時間 */
	UPROPERTY(EditAnywhere)
	float LifeTime;

	UPROPERTY(EditAnywhere)
	float OpacityDecayRate;

	float Opacity;

	/** 時間管理用 */
	float CurrentTime;

	/** 開始時の不透明度 */
	UPROPERTY(EditAnywhere, Category = "Material")
	float StartOpacity;

	/** 縁の色 */
	UPROPERTY(EditAnywhere, Category = "Material")
	FLinearColor StartColor;

	/** ベース色 */
	UPROPERTY(EditAnywhere, Category = "Material")
	FLinearColor StartBaseColor;
};
