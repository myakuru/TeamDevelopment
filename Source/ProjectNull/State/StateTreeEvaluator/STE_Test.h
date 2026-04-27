// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "STE_Test.generated.h"

// 敵クラス
class AEnemyBase;

/**
 * 敵のステートツリー
 */
UCLASS()
class PROJECTNULL_API USTE_Test : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
public:

	virtual void TreeStart(FStateTreeExecutionContext& Context)						override;
	virtual void TreeStop(FStateTreeExecutionContext& Context)						override;
	virtual void Tick(FStateTreeExecutionContext& Context, const float DeltaTime)	override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output)
	TObjectPtr<AEnemyBase> Enemy = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output)
	float Duration = 5.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Output)
	float Timer = 0.0f;
};