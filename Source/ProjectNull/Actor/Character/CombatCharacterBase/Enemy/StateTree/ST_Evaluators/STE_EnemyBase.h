#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "STE_EnemyBase.generated.h"

/**
* 敵クラス
*/
class AEnemyBase;

/**
 * 敵のステートツリーの基底クラス
 */
UCLASS()
class PROJECTNULL_API USTE_EnemyBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief ステートツリーの開始処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStart(FStateTreeExecutionContext& a_Context)						override;
	
	/**
	 * @brief ステートツリーの更新処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 * @param a_DeltaTime デルタタイム
	 */
	virtual void Tick(FStateTreeExecutionContext& a_Context,const float a_DeltaTime)	override;
	
	/**
	 * @brief ステートツリーの終了処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStop(FStateTreeExecutionContext& a_Context)						override;

	/**
	 * @brief デリゲートへの関数登録
	 */
	virtual void RegisterDelegate();

public:

	//~ Begin Setter
	
	/**
	 * @brief 移動方向のセット
	 * @param a_MoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& a_MoveDir) { MoveDir = a_MoveDir; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param a_DistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float a_DistSqr) { TargetDistanceSqr = a_DistSqr; }

	//~ End Setter

private:

	/**
	 * @brief 持ち主
	 */
	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy = nullptr;

	/**
	 * @brief 移動ベクトル
	 */
	UPROPERTY(VisibleAnywhere, Category = "Output",meta = (AllowPrivateAccess = "true"))
	FVector MoveDir = FVector::ZeroVector;

	/**
	 * @brief ターゲットとの距離の二乗値
	 */
	UPROPERTY(VisibleAnywhere,Category="Output", meta = (AllowPrivateAccess = "true"))
	float TargetDistanceSqr = 0.0f;
};
