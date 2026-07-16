#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyMidBossDeath.generated.h"

/** 中ボスのクラス */
class AEnemyMidBossBase;

/**
 * 中ボスの死亡ステート
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyMidBossDeath : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTT_EnemyMidBossDeath(const FObjectInitializer& InObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)					override;

private:

	/**	持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyMidBossBase>	OwnerEnemy = nullptr;

	/**	死亡アニメーション */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	TObjectPtr<UAnimMontage>		DeathAnim = nullptr;

	/**	死亡アニメーションのタイマー */
	UPROPERTY()
	float DeathAnimTimer = 0.f;

	/**	アニメーションが終了してから消滅するまでの遅延時間 */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float DeathDurationTime = 0.f;
};
