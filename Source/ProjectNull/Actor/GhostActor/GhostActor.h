
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


	void Update(float DeltaTime);

private:


	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

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
	FLinearColor StartRimColor;

};
