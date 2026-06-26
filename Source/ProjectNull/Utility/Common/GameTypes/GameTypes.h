#pragma once

#include "CoreMinimal.h"
#include "GameTypes.generated.h"

/** しきい値の最大値、最小値 */
USTRUCT(BlueprintType)
struct FThresholdRange
{
	GENERATED_BODY()

public:
	FThresholdRange():
		MinThreshold(0.0f),
		MaxThreshold(0.0f)
	{
	}
public:

	UPROPERTY(EditAnywhere)
	float MinThreshold;

	UPROPERTY(EditAnywhere)
	float MaxThreshold;

	/**
	 * @brief 範囲内かどうか
	 * @param Value 基準にする数値
	 * @return 範囲内ならtrue 範囲外ならfalse
	 */
	inline bool IsWithinRange(float Value) const
	{
		if (Value >= MinThreshold && Value < MaxThreshold)
		{
			return true;
		}
		return false;
	}

	/**
	 * @brief 長さを返す
	 * @return 閾値の長さ
	 */
	inline float GetRange() const {
		return MaxThreshold - MinThreshold;
	}

};


/** 演出カメラデータ */
USTRUCT(BlueprintType)
struct FCameraSequenceData
{
	GENERATED_BODY()
public:

	FCameraSequenceData() :
		Time(0.f),
		TargetArmLength(0.f),
		TargetRotator(FRotator::ZeroRotator),
		bPause(false)
	{
	}

public:

	/** 区間時間 */
	UPROPERTY(EditAnywhere)
	float Time;

	/** カメラとプレイヤーの距離 */
	UPROPERTY(EditAnywhere)
	float TargetArmLength;

	/** 目指すべきカメラ回転 */
	UPROPERTY(EditAnywhere)
	FRotator TargetRotator;

	/** 停止するかどうか */
	UPROPERTY(EditAnywhere)
	bool bPause;
};

/**
 * @brief プレイヤー・敵が持つ攻撃力に関する要素
 */
USTRUCT(BlueprintType)
struct FCharacterAttackData
{
	GENERATED_BODY()
public:

	/**
	 * @brief 最終的な攻撃力を算出
	 * @param InSkillPower 攻撃そのもの威力
	 * @return 最終的な攻撃力 
	 */
	float GetFinalAttackPower(const float InSkillPower)
	{
		return InSkillPower + AttackPowner * PowerScaling;
	}

public:

	/**	攻撃力 */
	UPROPERTY(EditAnywhere)
	float AttackPowner = 1.f;

	/**	倍率 */
	UPROPERTY(EditAnywhere)
	float PowerScaling = 1.f;
};

USTRUCT(BlueprintType)
struct FEnemyDistanceData
{
	GENERATED_BODY()
public:
	FEnemyDistanceData() :
		Enemy(nullptr),
		DistanceSq(0.f),
		ToEnemyVector(FVector::ZeroVector)
	{
	}
public:

	/** 敵の中間基底クラス */
	UPROPERTY()
	TWeakObjectPtr<class AEnemyBase> Enemy;

	/** 対象からの距離Sq */
	float DistanceSq;

	/** 対象からの距離Sq */
	FVector ToEnemyVector;

};