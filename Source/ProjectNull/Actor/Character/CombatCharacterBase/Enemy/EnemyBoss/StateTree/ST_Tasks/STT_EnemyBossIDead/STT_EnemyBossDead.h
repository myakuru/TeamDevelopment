#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossDead.generated.h"

/** プレイヤー未発見時の徘徊タスク。NavMesh上を点々と移動する。 */
UCLASS()
class PROJECTNULL_API USTT_EnemyBossDead : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossDead(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/** アニメーションモンタージュ*/
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UAnimMontage> DeathMontage;

	/** 死亡エフェクトの終了時間*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float DeathEffectDuration = 5.0f;

	/** 死亡エフェクトの開始時間*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float DeathEffectStart = 3.0f;

	/** 死亡時のマテリアル変色*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float DethMaterialDuration = 3.0f;

	float DeathCount = 0.0f;

	bool bDeathAnimationEnd = false;

	bool bDeathEffectFlg = false;

};
