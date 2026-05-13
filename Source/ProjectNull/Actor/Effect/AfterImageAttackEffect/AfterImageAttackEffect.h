
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AfterImageAttackEffect.generated.h"

class AGhostActor;
class UAnimationAsset;

/** 残像攻撃データ */
USTRUCT(BlueprintType)
struct FAfterImageAttackData
{
	GENERATED_BODY()
public:
	FAfterImageAttackData() :
		bSpawn(false),
		PoseTime(0.0f),
		Time(0.0f)
	{
	}

public:

	inline FVector CalcLocationOffset(float LerpAlpha) const {	
		return FMath::Lerp(StartLocationOffset, EndLocationOffset, LerpAlpha);
	}

	/** スポーンしているか */
	bool bSpawn;

	UPROPERTY()
	AGhostActor* GhostActor;

	UPROPERTY(EditAnywhere)
	FVector StartLocationOffset;

	UPROPERTY(EditAnywhere)
	FVector EndLocationOffset;

	UPROPERTY(EditAnywhere)
	FVector Scale;

	UPROPERTY(EditAnywhere)
	FRotator RotationOffset;

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	UPROPERTY(EditAnywhere)
	float Time;

	UPROPERTY(EditAnywhere)
	float TimeThreshold;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	float PoseTime;

};

UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UAfterImageAttackEffect final : public UObject
{
	GENERATED_BODY()
public:
	UAfterImageAttackEffect();
public:
	
	/** 初期化 */
	void Initialize();


	void Start();

	void Update(float DeltaTime,
				float ElapsedTime,
				const FTransform& PlayerTransform);


	float GetTotalTime();
	float GetMaxTime();

	UPROPERTY(EditAnywhere)
	int32 AfterImageNum;

	/** 残像攻撃データ */
	UPROPERTY(EditAnywhere)
	TArray<FAfterImageAttackData> AfterImageDataArray;

	/** スケルタルメッシュ */
	UPROPERTY(EditAnywhere, Category = "Asset")
	USkeletalMesh* SkeletalMesh;

	/** アニメーション */
	UPROPERTY(EditAnywhere, Category = "Asset")
	UAnimationAsset* AnimationAsset;

	UPROPERTY(EditAnywhere, Category = "Ghost")
	TSubclassOf<AGhostActor> GhostClass;
};
