#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyDead.generated.h"

class AEnemyBase;

/**
* 敵の死亡ステートタスク
* 死亡アニメーション、死亡エフェクト、アイテムドロップなどを行う
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyDead : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyDead(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	/** @brief 持ち主のアドレス*/
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;
};
