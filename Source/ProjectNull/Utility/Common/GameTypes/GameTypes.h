#pragma once

#include "CoreMinimal.h"

#include "GameTypes.generated.h"

//　しきい値の最大値、最小値
USTRUCT(BlueprintType)
struct FThresholdRange
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float minThreshold = 0.0f;

	UPROPERTY(EditAnywhere)
	float maxThreshold = 0.0f;

	//　範囲内かどうか
	bool IsWithinRange(float Value) const
	{
		if (Value >= minThreshold && Value < maxThreshold)
		{
			return true;
		}
		return false;
	}
};
