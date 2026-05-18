
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ModelAfterimageTrailEffect.generated.h"

/** ゴーストアクタークラス
* アクターオブジェクトの残像表現クラス */
class AGhostActor;

/** モデル残像エフェクトクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UModelAfterimageTrailEffect : public UObject
{
	GENERATED_BODY()
public:
	UModelAfterimageTrailEffect();
public:

	/**
	 * @brief 更新
	 * @param DeltaTime デルタタイム
	 * @param InSkeletalMesh スケルタルメッシュ
	 * @param InSnapshot スナップショット
	 */
	void Update(float DeltaTime,
				class USkeletalMesh* InSkeletalMesh,
				const FPoseSnapshot& InSnapshot);

	/**
	 * @brief このクラスで管理しているすべての残像オブジェクトをワールドから削除
	 */
	void AllDestroy();

	inline void SetOwnerActor(AActor* Owner) { OwnerActor = Owner; }
	inline void SetCanAddTrailPoint(bool bInCanAddTrailPoint) { bCanAddTrailPoint = bInCanAddTrailPoint; }

private:

	/**
	 * @brief 残像ポイントの追加処理
	 * @param InAnimPoseTime アニメーションを停止して描画する際のアニメーション時間閾値
	 */

	void AddAfterimageTrail(class USkeletalMesh* InSkeletalMesh,
		const FPoseSnapshot& InSnapshot);

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	/** ポイント追加してよいか */
	bool bCanAddTrailPoint;

	/** 追加ポイントのデータ配列 */
	UPROPERTY()
	TArray<AGhostActor*> TrailPointDataArray;

	/** 残像追加時間管理 */
	float TrailAddTimer;

	/** 残像の追加間隔 */
	UPROPERTY(EditAnywhere, Category = "Params")
	float TrailAddInterval;

	/** 残像の最大長さ */
	UPROPERTY(EditAnywhere, Category = "Params")
	int32 TrailMaxLength;

	/** スケルタルメッシュ */
	UPROPERTY(EditAnywhere, Category = "Asset")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	/** アニメーション */
	UPROPERTY(EditAnywhere, Category = "Asset")
	TObjectPtr<UAnimationAsset> AnimationAsset;

	/** 追加し、描画するゴーストアクタークラス */
	UPROPERTY(EditAnywhere, Category = "Class")
	TSubclassOf<AGhostActor> GhostClass;

	/** アニメーションを停止して描画する際のアニメーション時間閾値 */
	UPROPERTY(EditAnywhere)
	float AnimPoseTime;
};
