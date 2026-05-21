
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

	void Tick(float DeltaTime) override;

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh スケルタルメッシュ
	 * @param InSnapshot ポーズ
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					const FPoseSnapshot& InSnapshot);

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh スケルタルメッシュ
	 * @param InSnapshot ポーズ
	 * @param InLifeTime 寿命時間
	 * @param InOpacityDecayRate 透明度減少量
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					const FPoseSnapshot& InSnapshot,
					float InLifeTime,
					float InOpacityDecayRate);

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh スケルタルメッシュ
	 * @param Animation アニメーションアセット
	 * @param InPoseTime アニメーションを停止して描画する際のアニメーション時間閾値
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					float InPoseTime);

	/**
	 * @brief 初期化処理
	 * @param SkeletalMesh スケルタルメッシュ
	 * @param Animation アニメーションアセット
	 * @param InPoseTime アニメーションを停止して描画する際のアニメーション時間閾値
	 * @param InLifeTime 寿命時間
	 * @param InOpacityDecayRate 透明度減少量
	 */
	void Initialize(class USkeletalMesh* SkeletalMesh,
					class UAnimationAsset* Animation,
					float InPoseTime,
					float InLifeTime,
					float InOpacityDecayRate);

private:

	/**
	 * @brief 共通初期化処理
	 * @param SkeletalMesh スケルタルメッシュ
	 */
	void InitializeInternal(class USkeletalMesh* SkeletalMesh);

	/** ルートコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> Root;

	/** 残像表示用スケルタルメッシュ */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	/** 残像描画時に使用するベースマテリアル */
	UPROPERTY(EditAnywhere, Category = "Material")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	/** 残像描画用動的マテリアルインスタンス
		フェード値や色変更などを実行時に制御するために使用 */
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;

	/** 透過度のソート優先度 */
	UPROPERTY(EditAnywhere)
	int32 TranslucencySortPriority;

	/** 寿命時間 */
	UPROPERTY(EditAnywhere)
	float LifeTime;

	/** 時間管理用 */
	float CurrentTime;

	/** 透明度減少量 */
	UPROPERTY(EditAnywhere, Category = "Material")
	float OpacityDecayRate;

	/** 透明度 */
	float Opacity;

	/** 開始時の不透明度 */
	UPROPERTY(EditAnywhere, Category = "Material")
	float StartOpacity;

	/** リムライトの色 */
	UPROPERTY(EditAnywhere, Category = "Material")
	FLinearColor StartRimColor;

	/** リムライトの鮮明さ */
	UPROPERTY(EditAnywhere, Category = "Material")
	float RimSharpness;

	/** リムライトの強さ */
	UPROPERTY(EditAnywhere, Category = "Material")
	float RimStrength;
};
