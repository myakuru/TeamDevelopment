// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "StateTreeTaskPrintBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UStateTreeTaskPrintBase : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	UStateTreeTaskPrintBase(const FObjectInitializer& ObjectInitializer)
		:Super(ObjectInitializer)
	{
		/**
		* ～Tickを有効化～
		* Start()では遅いので
		* コンストラクタにてbShouldCallTickをtrueにする事で、
		* インスタンスデータが更新されようでしてTickが呼ばれるんだと。。
		*/
		bShouldCallTick = true;
	}

	// タスク系
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)					override;

	UPROPERTY(EditAnywhere,Category=Parameter,meta=(ToolTip="Test"))
	float TestFloat = 0.0f;

	UPROPERTY(EditAnywhere, Category = Parameter, meta = (ToolTip = "Test"))
	FVector TestVector = FVector::ZeroVector;

	float Timer = 0.0f;
};
