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

	USTT_EnemyWalk(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	/**
	 * @brief 移動に必要なパラメータの初期化
	 */
	void InitializeWalkParams();

	/**
	 * @brief 移動処理
	 */
	void Move(const float a_DeltaTime);

	/**
	 * @brief 坂道移動処理
	 */
	void MoveSlope();

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
	FVector CalculateNextActorLocation(const FVector& a_CurrentLocation, const FVector& a_MoveDir, float a_DeltaTime, float a_MoveSpeed);

private:

	/** @brief 持ち主のアドレス*/
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;

	/** @brief 移動ベクトル*/
	UPROPERTY(EditAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

	/**	移動速度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float MoveSpeed = 300.0f;

	/**	回転速度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float RotSpeed = 5.0f;

	/**	歩くことのできる床の角度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float WarkableFloorAngle = 0.0f;

	/**	超える事の出来る段差の高さ */
	UPROPERTY(EditAnywhere, Category = "Input")
	float MaxStepHeight = 0.0f;

	/**	カプセルの高さ半径(高さ補正で使う) */
	UPROPERTY(EditAnywhere, Category = "Input")
	float CapsuleHalfHeight = 0.0f;

	/**	重力の速度 */
	UPROPERTY()
	FVector GravityVelocity = FVector::ZeroVector;

	/** 以前触れた地面の法線 */
	UPROPERTY()
	FVector AfterGroundNormal = FVector::ZeroVector;

	/**	接地フラグ */
	UPROPERTY()
	bool isGround = false;
};
