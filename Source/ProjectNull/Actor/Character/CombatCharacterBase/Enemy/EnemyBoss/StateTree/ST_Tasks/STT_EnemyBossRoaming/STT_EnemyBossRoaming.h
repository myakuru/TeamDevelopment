#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossRoaming.generated.h"

/** プレイヤー未発見時の徘徊タスク。NavMesh上を点々と移動する。 */
UCLASS()
class PROJECTNULL_API USTT_EnemyBossRoaming : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossRoaming(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime) override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) override;

	/** 徘徊半径 */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float RoamRadius = 800.0f;

	/** 到着とみなす距離 */
	UPROPERTY(EditAnywhere, Category = "Parameter")
	float AcceptanceRadius = 100.0f;

private:

	bool PickNewDestination();

	FVector CurrentDestination = FVector::ZeroVector;
	bool bHasDestination = false;

};