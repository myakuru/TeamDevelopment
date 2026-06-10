#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "StateTreeEvaluatorBase.h"
#include "STE_EnemyBossBase.generated.h"

// ボスクラス
class AEnemyBossBase;

UCLASS()
class PROJECTNULL_API USTE_EnemyBossBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()

public:

	virtual void TreeStart(FStateTreeExecutionContext& Context)override;
	virtual void TreeStop(FStateTreeExecutionContext& Context)override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime)override;


	/** 自分自身（StateTreeを持つボス）。TreeStartで一度だけ確定*/ 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Output")
	TObjectPtr<AEnemyBossBase> OwnerEnemyBoss = nullptr;

	/** AIController*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Output")
	TObjectPtr<AController> AIC = nullptr;

	/** 現在の追尾対象。ボスから毎フレーム読み取り、Conditions,Tasksへ配布*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Output")
	TObjectPtr<AActor> TargetActor = nullptr;
};
