
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AfterImageAttackEffect.generated.h"

class AGhostActor;
class UAnimationAsset;
class UModelAfterimageTrailEffect;

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
		MoveTime(0.0f),
		TimeThreshold(0.0f),
		LifeTime(0.0f),
		PoseTime(0.0f),
		OpacityDecayRate(1.0f)
	{
	}

public:

	inline FVector CalcLocationOffset(float LerpAlpha) const {	
		return FMath::Lerp(StartLocationOffset, EndLocationOffset, LerpAlpha);
	}

	/** モデル残像エフェクトクラス */
	UPROPERTY(EditAnywhere,Instanced)
	TObjectPtr<UModelAfterimageTrailEffect> ModelAfterimageTrailEffect;

	/** スポーンしているか */
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

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY(EditAnywhere)
	float TimeThreshold;

	UPROPERTY(EditAnywhere)
	float LifeTime;

	UPROPERTY(EditAnywhere)
	float OpacityDecayRate;
	
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


	float GetMaxTime();

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
	FTransform StartTransfrom;

};
