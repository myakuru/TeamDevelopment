#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyKnockBack.generated.h"

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

protected:

	/**	ノックバック速度 */
	UPROPERTY()
	FVector	KnockBackVelocity = FVector::ZeroVector;

	/**	敵の重量 */
	UPROPERTY(VisibleAnywhere, Category = "Input")
	float	KnockBackWeight	= 1.0f;

};
