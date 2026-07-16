
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AfterImageAttackEffect.generated.h"

/** アクターオブジェクトの残像表現クラス */
class AGhostActor;

/** アニメーションアセットクラス */
class UAnimationAsset;

/** モデル残像エフェクトクラス */
class UModelAfterimageTrailEffect;

/** 効果音　*/
class USoundBase;

/** 残像攻撃データ */
USTRUCT(BlueprintType)
struct FAfterImageAttackData
{
	GENERATED_BODY()
public:
	FAfterImageAttackData() :
		ModelAfterimageTrailEffect(nullptr),
		bSpawn(false),
		StartLocationOffset(FVector::ZeroVector),
		EndLocationOffset(FVector::ZeroVector),
		Scale(FVector::OneVector),
		RotationOffset(FRotator::ZeroRotator),
		Transform(FTransform()),
		MoveTime(0.f),
		TimeThreshold(0.f),
		LifeTime(0.f),
		OpacityDecayRate(1.f),
		PoseTime(0.f)
	{
	}

public:

	/**
	 * @brief 制御時間内かどうか
	 * @param ElapsedTime 経過時間
	 * @return 制御時間内ならtrue 制御時間外ならfalse
	 */
	inline bool IsWithinTimeRange(float ElapsedTime) const
	{
		return (ElapsedTime >= TimeThreshold)
			&& (GetCurrentTime(ElapsedTime) <= MoveTime);
	}

	/**
	 * @brief 現在の経過時間取得
	 * @param ElapsedTime 経過時間
	 * @return 現在の経過時間
	 */
	inline float GetCurrentTime(float ElapsedTime) const
	{
		return ElapsedTime - TimeThreshold;
	}

	/**
	 * @brief 座標オフセット計算(LerpAlphaに基づいて補間) 直線移動を行う
	 * @param LerpAlpha 補間値
	 * @return 計算結果
	 */
	inline FVector CalcLocationOffset(float LerpAlpha) const {	
		return FMath::Lerp(StartLocationOffset, EndLocationOffset, LerpAlpha);
	}

	/**
	 * @brief 移動方向を計算
	 * @return 計算結果
	 */
	inline FVector CalcMoveDir() const {	
		return (EndLocationOffset - StartLocationOffset).GetSafeNormal();
	}

	
	/**
	 * @brief 残像をスポーンするかどうかのフラグをセットする
	 * @param bInEnableSpawn 
	 */
	bool SetEnableSpawn(bool bInEnableSpawn) const;
	

	/** モデル残像エフェクトクラス */
	UPROPERTY(EditAnywhere,Instanced)
	TObjectPtr<UModelAfterimageTrailEffect> ModelAfterimageTrailEffect;

	/** スポーンしているか 
		Runtime値 */
	bool bSpawn;

	/** 移動する際の開始座標(オフセット) */
	UPROPERTY(EditAnywhere)
	FVector StartLocationOffset;

	/** 移動する際の終了座標(オフセット) */
	UPROPERTY(EditAnywhere)
	FVector EndLocationOffset;

	/** 拡大率 */
	UPROPERTY(EditAnywhere)
	FVector Scale;

	/** 回転オフセット */
	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	/** トランスフォーム情報 */
	UPROPERTY()
	FTransform Transform;

	/** 残像が始点から終点まで移動する時間 */
	UPROPERTY(EditAnywhere)
	float MoveTime;

	/** 残像を制御し始める時間閾値 */
	UPROPERTY(EditAnywhere)
	float TimeThreshold;

	/** 残像の寿命時間 */
	UPROPERTY(EditAnywhere)
	float LifeTime;

	/** 残像の透明度減少量 */
	UPROPERTY(EditAnywhere)
	float OpacityDecayRate;
	
	/** アニメーションアセットの停止時間閾値) */
	UPROPERTY(EditAnywhere, Category = "Animation")
	float PoseTime;
};

/** 残像攻撃(連続的に描画する残像)の振る舞いを管理するエフェクトクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UAfterImageAttackEffect final : public UObject
{
	GENERATED_BODY()
public:
	UAfterImageAttackEffect();
	
	/** 初期化 */
	void Initialize();

	/**
	 * @brief 開始処理(開始のフレームだけ呼ばれる)
	 */
	void Start(const FTransform& Transform);

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 */
	void Update(float DeltaTime,
				float ElapsedTime);

	/**
	 * @brief 経過時間に対しての残像制御終了最大値
	 * @return 計算結果
	 */
	float GetMaxTime();
	
	void SetSESound(USoundBase* inSound);

private:

	/**
	 * @brief 残像の構造体データ更新
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 * @param Data 残像の構造体データ
	 */
	void UpdateAfterimageAttackData(float DeltaTime, float ElapsedTime, FAfterImageAttackData& Data);

	/** 残像攻撃データ配列 */
	UPROPERTY(EditAnywhere)
	TArray<FAfterImageAttackData> AfterImageDataArray;

	/** 残像のスケルタルメッシュ */
	UPROPERTY(EditAnywhere, Category = "Asset")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	/** 残像アニメーション */
	UPROPERTY(EditAnywhere, Category = "Asset")
	TObjectPtr<UAnimationAsset> AnimationAsset;

	/** 攻撃開始時に基準とするトランフォーム情報 */
	FTransform StartTransfrom;
	
	/** 効果音 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<USoundBase> DashSESound;
};
