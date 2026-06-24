#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyKnockBack.generated.h"

class AEnemyBase;

/** ノックバックに必要な要素 */
class UKnockBackData;

/**
 * 敵のノックバックステート
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyKnockBack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyKnockBack(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	/**
	 * @brief ノックバックに必要な情報を設定
	 */
	void SetKnockBackData();

	/**
	 * @brief ノックバック更新処理
	 * @param a_DeltaTime デルタタイム
	 * @return ノックバック停止時はtrue
	 */
	bool MoveToKnockBack(const float a_DeltaTime);

protected:

	/** 持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy = nullptr;

	/**	ノックバックに必要な情報を格納しているデータアセット */
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UKnockBackData> KnockBackData = nullptr;

	/**	ノックバック速度 */
	UPROPERTY()
	FVector	KnockBackVelocity = FVector::ZeroVector;

	/**	移動方向 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

	/**	ターゲットの座標 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	FVector TargetLocation = FVector::ZeroVector;

	/**	敵の重量 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	float	EnemyWeight = 1.0f;

	/**	最大体力に対する受けたダメージの割合 */
	UPROPERTY()
	float	DamageRatio = 0.0f;

private:

	/**
	 * @brief デバッグ用攻撃倍率
	 */
	UPROPERTY(EditAnywhere)
	float DebugDamageRatio = 1.f;
};
