#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyParameterData.generated.h"

/** ターゲットとの距離が変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnTargetDistChanged,const FVector& /*OwnerPos*/,float /*DistSqr*/);


/**
 * Enemyの処理に必要なパラメータを管理するクラス
 */
UCLASS()
class PROJECTNULL_API UEnemyParameterData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UEnemyParameterData();

	//~ Begin Setter

	/**
	 * @brief				ターゲットとの距離計算処理
	 * @param a_TargetPos	ターゲット座標
	 * @param a_OwnerPos	原点座標
	 */
	UFUNCTION(BlueprintCallable,Category="EnemyPara")
	void CalcDistanceToTarget(const FVector& a_TargetPos, const FVector& a_OwnerPos);

	//~ End Setter

	/** ターゲットとの距離が変更された時に呼び出される */
	FOnTargetDistChanged OnTargetDistChanged;

private:

	/** 進行ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "EnemyPara")
	FVector MoveDir = FVector::ZeroVector;

	/** ターゲットとの距離(Sqr) */
	UPROPERTY(VisibleAnywhere, Category = "EnemyPara")
	float TargetDistanceSqr = 0.0f;
};
