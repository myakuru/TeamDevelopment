#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyKnockBack.generated.h"

class AEnemyBase;

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
	 */
	void MoveToKnockBack(const float a_DeltaTime);

protected:

	/** 持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy = nullptr;

	/**	ノックバックに必要な情報を格納しているデータテーブル */
	UPROPERTY(EditAnywhere, Category = "Data")
	TObjectPtr<UDataTable> KnockBackDataTable;

	/**	ノックバック速度 */
	UPROPERTY()
	FVector	KnockBackVelocity = FVector::ZeroVector;

	/**	移動方向 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

	/**	受けた攻撃力 */
	UPROPERTY()
	float	ReceivedAttackPower = 0.0f;

	/**	敵の重量 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	float	EnemyWeight	= 1.0f;
};
