
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "ModelAfterimageTrailEffect.generated.h"

/** ゴーストアクタークラス
* アクターオブジェクトの残像表現クラス */
class AGhostActor;

/** モデル残像エフェクトクラス
	残像を連続的に描画するエフェクトクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UModelAfterimageTrailEffect : public UObject
{
	GENERATED_BODY()
public:
	UModelAfterimageTrailEffect();
public:

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 * @param InTransform 残像のトランフォーム
	 * @param InSkeletalMesh 残像のスケルタルメッシュ
	 * @param InSnapshot 残像のポーズ
	 */
	void Update(float DeltaTime,
				const FTransform& InTransform,
				class USkeletalMesh* InSkeletalMesh,
				const FPoseSnapshot& InSnapshot);

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 * @param InTransform 残像のトランフォーム
	 * @param InSkeletalMesh 残像のスケルタルメッシュ
	 * @param InAnimation 残像のアニメーションアセット
	 * @param InPoseTime 停止したいアニメーション閾値
	 */
	void Update(float DeltaTime,
				const FTransform& InTransform,
				class USkeletalMesh* InSkeletalMesh,
				class UAnimationAsset* InAnimation,
				float InPoseTime);

	/**
	 * @brief このクラスで管理しているすべての残像オブジェクトをワールドから削除
	 */
	void AllDestroy();

	/** Getter */
	inline bool EnableSpawn() const { return bEnableSpawn; }

	/** Setter */
	inline void SetEnableSpawn(bool bInEnableSpawn) { bEnableSpawn = bInEnableSpawn; }

private:

	/**
	 * @brief 更新共通処理
	 * @param DeltaTime デルタタイム
	 * @param InTransform 残像のトランフォーム
	 * @param InSkeletalMesh 残像のスケルタルメッシュ
	 * @param InitializeFunc ゴーストアクター初期化メソッド
	 */
	void UpdateInternal(float DeltaTime,
						const FTransform& InTransform,
						USkeletalMesh* InSkeletalMesh,
						TFunction<void(AGhostActor*)> InitializeFunc);

	/**
	 * @brief 残像の追加処理
	 * @param InTransform 残像のトランフォーム
	 * @param InitializeFunc ゴーストアクター初期化メソッド
	 */
	void AddAfterimageTrail(const FTransform& InTransform,
							TFunction<void(AGhostActor*)> InitializeFunc);

	/**
	 * @brief 最大保持数を超えた残像を削除
	 * 古い残像から順にDestroyする
	 */
	void DestroyOverflowTrail();


	/** 残像をスポーンするかどうか */
	bool bEnableSpawn;

	/** 残像をまとめる配列 */
	UPROPERTY()
	TArray<TWeakObjectPtr<AGhostActor>> GhostActors;

	/** 残像追加時間管理 */
	float TrailAddTimer;

	/** 残像の追加間隔 */
	UPROPERTY(EditAnywhere, Category = "Params")
	float SpawnInterval;

	/** 残像の最大長さ */
	UPROPERTY(EditAnywhere, Category = "Params")
	int32 TrailMaxLength;

	/** このクラスで制御する残像クラス */
	UPROPERTY(EditAnywhere, Category = "Class")
	TSubclassOf<AGhostActor> GhostClass;
};
