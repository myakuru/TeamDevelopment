#pragma once

#include "AnimDataAsset.generated.h"


USTRUCT()
struct FAnimData
{
	GENERATED_BODY()

	/** テクスチャーのアニメーションの開始位置*/
	UPROPERTY(EditDefaultsOnly)
	float StartTime = 0.0f;

	/** アニメーションのフレーム*/
	UPROPERTY(EditDefaultsOnly)
	float NumFrames = 60.0f;
};

UCLASS()
class UEnemyAnimDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	/** アニメーションデータ数のデータ*/
	UPROPERTY(EditDefaultsOnly)
	TArray<FAnimData> Animations;
};