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


protected:

	/**
	 * @brief 持ち主
	 */
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;

	/**
	 * @brief プレイヤーへの移動ベクトル
	 */
	UPROPERTY(VisibleAnywhere, Category = "Output")
	FVector MoveDir = FVector::ZeroVector;

};
