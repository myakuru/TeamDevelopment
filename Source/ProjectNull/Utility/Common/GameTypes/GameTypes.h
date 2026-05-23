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