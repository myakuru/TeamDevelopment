// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyWalk.generated.h"

/**
 * 持ち主クラス
 */
class AEnemyBase;

/**
 * 敵の移動ステート
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyWalk : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTT_EnemyWalk(const FObjectInitializer& a_ObjInit)
		: Super(a_ObjInit)
	{
		/*
		* Tick処理有効化
		*/
		bShouldCallTick = true;
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	/**
	 * @brief 移動処理
	 */
	void Move(const float a_DeltaTime);

	/**
	 * @brief 回転補間処理
	 * @param a_CurrentRot	現在の角度
	 * @param a_TargetRot	目標角度
	 * @param a_RotSpeed	回転スピード
	 * @param a_DeltaTime	デルタタイム
	 * @return 補間結果
	 */
	FRotator CalculateRotationToMoveDir(const FRotator& a_CurrentRot, const FRotator& a_TargetRot, float a_RotSpeed, float a_DeltaTime);

	/**
	 * @brief 座標更新計算
	 * @param a_MoveDir		移動方向
	 * @param a_MoveSpeed	移動速度
	 * @param a_DeltaTime	デルタタイム
	 * @return	更新結果 
	 */
	FVector CalculateNextActorLocation(const FVector& a_CurrentLocation,const FVector& a_MoveDir, float a_MoveSpeed, float a_DeltaTime);

private:

	/** @brief 持ち主のアドレス*/
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;

	/** @brief 移動ベクトル*/
	UPROPERTY(EditAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

};
