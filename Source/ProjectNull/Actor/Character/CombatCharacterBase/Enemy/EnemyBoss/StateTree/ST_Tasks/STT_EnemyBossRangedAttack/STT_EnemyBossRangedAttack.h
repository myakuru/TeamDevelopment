#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossRangedAttack.generated.h"

class AEnemyBossBase;

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossRangedAttack : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossRangedAttack(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	/** 現在の選択されている攻撃パターン*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FBossAttackPattern CurrentAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MoveSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float Elapsed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float StrafeTime = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Input")
	float StrafeSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float StrifeDir = 1.0f;
};