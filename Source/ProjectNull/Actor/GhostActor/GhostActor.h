
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
					const FPoseSnapshot& InSnapshot);

	void Initialize(class USkeletalMesh* SkeletalMesh,
					const FPoseSnapshot& InSnapshot,
					float InLifeTime,
					float InOpacityDecayRate);

	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					float InPoseTime,
					float InLifeTime,
					float InOpacityDecayRate);


	void Update(float DeltaTime);

private:

	void Initialize(class USkeletalMesh* SkeletalMesh);


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	/** 透過度のソート優先度 */
	UPROPERTY(EditAnywhere)
	int32 TranslucencySortPriority;

	/** 描画時間 */
	UPROPERTY(EditAnywhere)
	float LifeTime;

	/** 時間管理用 */
	float CurrentTime;

	/** 開始時の不透明度 */
	UPROPERTY(EditAnywhere, Category = "Material")
	float StartOpacity;

	/** 縁の色 */
	UPROPERTY(EditAnywhere, Category = "Material")
	FLinearColor StartRimColor;

	/** 縁の鮮明さ */
	UPROPERTY(EditAnywhere, Category = "Material")
	float RimSharpness;

	/** 縁の強さ */
	UPROPERTY(EditAnywhere, Category = "Material")
	float RimStrength;

	UPROPERTY(EditAnywhere, Category = "Material")
	float OpacityDecayRate;

	float Opacity;
};
